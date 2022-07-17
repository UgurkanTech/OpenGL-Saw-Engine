#include "GUI.h"
#include "GLEngine.h"
bool test = false;
void GUI::drawGUI() {
	int x = 50, y = 50, w = 200, h = 100;

	if(test)
		GLEngine::renderer->renderSprite(GLEngine::shaders.sprite, "button", x, y, w, h, glm::vec4(0.5, 0.1, 0.5, 0.8));
	else
		GLEngine::renderer->renderSprite(GLEngine::shaders.sprite, "button", x, y, w, h, glm::vec4(1, 1, 1, 0.6));
	
	GLEngine::renderer->renderText(GLEngine::shaders.textShader, "This is a button", 60, 75, 1.0f, glm::vec3(0.7, 0.7f, 0.7f));


	for (size_t i = 0; i < 10; i++)
	{
		GLEngine::renderer->renderSprite(GLEngine::shaders.sprite, "", 400, 200 + i * 20, 400, 3, glm::vec4(1, 0, 0, 1));

	}
}

void GUI::clickEvent(double posX, double posY, bool rightClick ) {
	if (checkCollision(posX, posY)){
		std::cout << "Button clicked" << std::endl;
		//std::cout << "" << std::endl;

		GLEngine::enableSkybox = !GLEngine::enableSkybox;
		//gl->scene->move = !value;
	}
		
}

bool GUI::FindPoint(int x1, int y1, int x2, int y2, int x, int y)
{
	if (x > x1 && x < x2 && y > y1 && y < y2)
		return true;
	return false;
}

bool GUI::checkCollision(double posX, double posY) {
	return FindPoint(50, 50, 250, 150, posX, posY);
}
void GUI::positionEvent(double posX, double posY) {
	test = FindPoint(50, 50, 250, 150, posX, posY);
}