#pragma once
#include "Textures.h"
#include "Renderer.h"
using namespace std;

class Shader;
class Transform {
public:
	
	glm::mat4 model;

	Shape shape = Shape::Cube;

	Shader* shader;
	Textures* textures;

	Transform(){}
	Transform(Shader* shader, string textureName, Shape shape = Shape::Cube, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 scale = glm::vec3(1, 1, 1));

	void translateModel(glm::vec3 vec);

	void scaleModel(glm::vec3 vec);

	void scaleModel(int ratio);

	void activateTexture();

	void renderModel(bool enableTexture = true, bool enableShader = true);
};