#include "Scene.h"
#include "GLEngine.h"
void Scene::loadTextureNames(bool viewLoadingScreen) {
	for (size_t i = 0; i < assetNames.size(); i++) {
		GLEngine::tm->addTextures(assetNames.at(i).assetName, assetNames.at(i).assetPath);
		if (viewLoadingScreen)
			viewLoadScreen(static_cast<double>(i) / assetNames.size() * 100, "Loading Textures - ");
	}
	viewLoadScreen(90, "Loading Background - ");
	stbi_set_flip_vertically_on_load(true);
	HDRBackgroundData = stbi_loadf(FileSystem::getPath(HDRBackgroundPath).c_str(), &HDRBackgroundWidth, &HDRBackgroundHeight, &HDRBackgroundNrComponents, 0);

}
void Scene::viewLoadScreen(int percentage, string text) {
	GLEngine::shaders.textShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLEngine::renderer->renderText(GLEngine::shaders.textShader, text + to_string(percentage) + "%", 100.0, GLEngine::SCR_HEIGHT / 2, 1.0f, glm::vec3(0.9, 0.0f, 0.2f));
	glfwSwapBuffers(GLEngine::window);
}