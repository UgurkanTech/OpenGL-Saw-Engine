#ifndef GLENGINE
#define GLENGINE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "filesystem.h"
#include "shader.h"
#include "camera.h"

#include <ft2build.h>
#include <freetype\freetype.h>
#include <iostream>
#include "TextureManager.h"

#include "Scene.h"
#include "Callbacks.h"

#include "Textures.h"
#include "Renderer.h"
#include <vector>
#include <map>
#include "Scene.h"
#include "GUI.h"
#include "LightSource.h"
#include "Transform.h"
class GLEngine {
private:
	// timing
	double deltaTime = 0;
	int fps, ticks;
	bool trigger = false;
	

	unsigned int irradianceMap;
	unsigned int envCubemap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;
public:
	// settings
	static bool D3;
	static const unsigned int SCR_WIDTH = 1280;
	static const unsigned int SCR_HEIGHT = 720;
	static double SCR_Scale_X;
	static double SCR_Scale_Y;
	// camera
	static Renderer* renderer;
	static Camera camera;
	static GUI* gui;
	static Callbacks cb;
	static GLFWwindow* window;
	static Shader::Shaders shaders;
	static Scene* scene;
	static TextureManager* tm;
	GLEngine();
	int initEngine(Scene* scene);

	static bool enableSkybox;
	static const bool viewLightLocations = true;
	
	void initShaders();
	int initText();
	void initFiltersAndHDR();
	void initLoop();
	void fixedGameLoop();
	void gameLoop();
	void renderGraphics();
	void renderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);
	void processInput();
};

#endif
