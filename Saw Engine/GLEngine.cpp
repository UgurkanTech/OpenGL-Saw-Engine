#include "GLEngine.h"

//statics
bool GLEngine::enableSkybox = false;
Renderer* GLEngine::renderer = NULL;
Shader::Shaders GLEngine::shaders;
TextureManager* GLEngine::tm = NULL;
Camera GLEngine::camera;
GUI* GLEngine::gui = NULL;
double GLEngine::SCR_Scale_X = 1;
double GLEngine::SCR_Scale_Y = 1;
bool GLEngine::D3 = false;
Scene* GLEngine::scene = NULL;
GLFWwindow* GLEngine::window = NULL;
Callbacks GLEngine::cb;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void GLEngine::processInput()
{
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (!D3) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
}

GLEngine::GLEngine() {
	cout << "GLEngine started!" << endl;
}

void GLEngine::initLoop() {
	scene->initLoop();
}

void GLEngine::fixedGameLoop() {
	scene->fixedGameLoop();
}
void GLEngine::gameLoop() {
	scene->gameLoop();
}

void GLEngine::initShaders() {
	// build and compile shaders
	// -------------------------
	shaders.defaultShader = new Shader("default.vs", "default.fs");
	shaders.billboard = new Shader("billboard.vs", "billboard.fs");
	shaders.pbrShader = new Shader("2.2.2.pbr.vs", "2.2.2.pbr.fs");
	shaders.equirectangularToCubemapShader = new Shader("2.2.2.cubemap.vs", "2.2.2.equirectangular_to_cubemap.fs");
	shaders.irradianceShader = new Shader("2.2.2.cubemap.vs", "2.2.2.irradiance_convolution.fs");
	shaders.prefilterShader = new Shader("2.2.2.cubemap.vs", "2.2.2.prefilter.fs");
	shaders.brdfShader = new Shader("2.2.2.brdf.vs", "2.2.2.brdf.fs");
	shaders.backgroundShader = new Shader("2.2.2.background.vs", "2.2.2.background.fs");
	shaders.textShader = new Shader("text.vs", "text.fs");
	shaders.sprite = new Shader("sprite.vs", "sprite.fs");

	shaders.pbrShader->use();
	shaders.pbrShader->setInt("irradianceMap", 0);
	shaders.pbrShader->setInt("prefilterMap", 1);
	shaders.pbrShader->setInt("brdfLUT", 2);
	shaders.pbrShader->setInt("albedoMap", 3);
	shaders.pbrShader->setInt("normalMap", 4);
	shaders.pbrShader->setInt("metallicMap", 5);
	shaders.pbrShader->setInt("roughnessMap", 6);
	shaders.pbrShader->setInt("aoMap", 7);

	shaders.defaultShader->use();
	shaders.defaultShader->setVec2("u_resolution", 2048, 2048);

	shaders.billboard->use();
	shaders.billboard->setInt("albedoMap", 3);

	shaders.backgroundShader->use();
	shaders.backgroundShader->setInt("environmentMap", 0);

	glm::mat4 projection2 = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	shaders.textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(shaders.textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection2));
	shaders.sprite->use();
	glUniformMatrix4fv(glGetUniformLocation(shaders.sprite->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection2));

}

int GLEngine::initText() {
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl; return -1;
	}

	// find path to font
	std::string font_name = FileSystem::getPath("resources/fonts/HAMBH.ttf");
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl; return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) { std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; return -1; }
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 32);
		renderer->characterHeight = 32;
		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl; continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Renderer::Character character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x) };
			renderer->Characters.insert(std::pair<char, Renderer::Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}


void GLEngine::initFiltersAndHDR() {

	// pbr: setup framebuffer
	// ----------------------

	int quality = 1024;

	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, quality, quality);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// pbr: load the HDR environment map
	// ---------------------------------
	
	unsigned int hdrTexture;
	if (scene->HDRBackgroundData)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scene->HDRBackgroundWidth, scene->HDRBackgroundHeight, 0, GL_RGB, GL_FLOAT, scene->HDRBackgroundData); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(scene->HDRBackgroundData);
	}
	else { std::cout << "Failed to load HDR image." << std::endl; }

	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------

	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, quality, quality, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	shaders.equirectangularToCubemapShader->use();
	shaders.equirectangularToCubemapShader->setInt("equirectangularMap", 0);
	shaders.equirectangularToCubemapShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, quality, quality); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		shaders.equirectangularToCubemapShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	// --------------------------------------------------------------------------------

	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);


	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	shaders.irradianceShader->use();
	shaders.irradianceShader->setInt("environmentMap", 0);
	shaders.irradianceShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		shaders.irradianceShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	// --------------------------------------------------------------------------------

	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	// ----------------------------------------------------------------------------------------------------
	shaders.prefilterShader->use();
	shaders.prefilterShader->setInt("environmentMap", 0);
	shaders.prefilterShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shaders.prefilterShader->setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			shaders.prefilterShader->setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderer->renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// pbr: generate a 2D LUT from the BRDF equations used.
	// ----------------------------------------------------

	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	shaders.brdfShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


// lights
// ------
LightSource* lights[128];
int lightsSize;

Transform* transforms[16384];
int transformsSize;

int GLEngine::initEngine(Scene* scene) {
	this->scene = scene;
	// glfw: initialize and configure
// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4); //4x msaa
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Saw Engine", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//camera = new Camera();
	
	camera.Position = glm::vec3(0.0f, 0.0f, 10.0f);

	//Handle events
	cb.init(window);

	// tell GLFW to capture our mouse
	if(D3)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	renderer = new Renderer();
	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &renderer->VAO);
	glGenBuffers(1, &renderer->VBO);
	glBindVertexArray(renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	initShaders();
	
	tm = new TextureManager;
	tm->loadNullTextures();
	gui = new GUI;
	
	tm->loadNullTextures();
	tm->nullShader = shaders.defaultShader;

	initText();
	//gui->gl = this;
	scene->loadResources();
	initFiltersAndHDR();


	cout << "size(mb):" << to_string(sizeof(transforms) / 1048576.0) << endl;

	transformsSize = scene->transforms.size();
	lightsSize = scene->lightSources.size();

	std::copy(scene->transforms.begin(), scene->transforms.end(), transforms);
	std::copy(scene->lightSources.begin(), scene->lightSources.end(), lights);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shaders.backgroundShader->use();
	shaders.backgroundShader->setMat4("projection", projection);

	shaders.pbrShader->use();
	shaders.pbrShader->setMat4("projection", projection);
	shaders.pbrShader->setInt("lightCount", lightsSize);

	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);


	static double limitFPS = 1.0 / 30.0; //Physics fps

	double lastTime = glfwGetTime(), nowTime = 0, timer = 0, delta = 0;

	int frames = 0, fixedUpdates = 0;
	int averageFPS[10]{};

	// render loop
	// -----------
	initLoop();
	while (!glfwWindowShouldClose(window))
	{
		//Time for frames
		nowTime = glfwGetTime();
		delta += (nowTime - lastTime) / limitFPS;
		deltaTime = nowTime - lastTime;
		lastTime = nowTime;

		// - Only update at 60 frames / s
		while (delta >= 1.0) {
			fixedGameLoop();   // - Update function
			fixedUpdates++;
			delta--;
		}
		// - Render at maximum possible frames
		processInput();
		gameLoop();
		renderGraphics();
		frames++;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		// - Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			averageFPS[(int)glfwGetTime() % 10] = frames;
			double avg = 0;
			for (size_t i = 0; i < 10; i++)
			{
				avg += averageFPS[i];
			}
			avg /= 10;

			//std::cout << "Render FPS: " << frames << " Fixed Updates:" << fixedUpdates << " Avg:" << avg << std::endl;
			fps = frames;
			ticks = fixedUpdates;
			fixedUpdates = 0, frames = 0;
		}
	}
	glfwTerminate();
	return 0;
}

string round2(string var)
{
	return var.substr(0, var.find(".") + 3);
}


void GLEngine::renderGraphics() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//View
	glm::mat4 view = camera.GetViewMatrix();
	//Zoom
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//shaders.backgroundShader->use();
	//shaders.backgroundShader->setMat4("projection", projection);


	shaders.defaultShader->use();
	shaders.defaultShader->setMat4("view", view);
	shaders.defaultShader->setMat4("projection", projection);

	shaders.billboard->use();
	shaders.billboard->setVec3("camPos", this->camera.Position);
	shaders.billboard->setMat4("view", view);
	shaders.billboard->setMat4("projection", projection);

	shaders.pbrShader->use();
	shaders.pbrShader->setVec3("camPos", this->camera.Position);
	shaders.pbrShader->setMat4("view", view);
	shaders.pbrShader->setMat4("projection", projection);

	// bind pre-computed IBL data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

	int lastID = -1;
	for (size_t i = 0; i < transformsSize; i++) {
		if (transforms[i]->shader->ID != lastID) { //If last shader is NOT same
			transforms[i]->renderModel(true, true); //Enable shader
			lastID = transforms[i]->shader->ID; //Change
		}
		else
		{
			transforms[i]->renderModel(true, false);
		}
	}

	// keeps the codeprint small.
	shaders.pbrShader->use();
	for (unsigned int i = 0; i < lightsSize; ++i)
	{
		shaders.pbrShader->setVec3("lightPositions[" + std::to_string(i) + "]", lights[i]->position);
		shaders.pbrShader->setVec3("lightColors[" + std::to_string(i) + "]", lights[i]->color);

		if (viewLightLocations) {
			Transform t(shaders.pbrShader, "gold", Shape::Sphere, lights[i]->position, glm::vec3(0.5, 0.5, 0.5));
			shaders.billboard->setVec3("spriteColor", glm::vec3(1, .5, 0.2));
			shaders.billboard->setVec4("vertex", glm::vec4(0, 0, 0, 0));
			t.renderModel();
		}
	}
	//enableSkybox = false;
	if (enableSkybox) {
		// render skybox (render as last to prevent overdraw)
		shaders.backgroundShader->use();

		shaders.backgroundShader->setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
		//glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
		renderer->renderCube();
	}

	// render BRDF map to screen
	//shaders.brdfShader->use();
	//renderer->renderCube();

	//GUI
	shaders.textShader->use();
	renderer->renderText(shaders.textShader, "FPS:" + to_string(fps) + " Ticks:" + to_string(ticks), SCR_WIDTH - 150, 15, 0.7f, glm::vec3(0.3, 0.7f, 0.9f));
	gui->drawGUI();
}