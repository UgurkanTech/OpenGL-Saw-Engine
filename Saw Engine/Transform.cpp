#include "Transform.h"
#include "GLEngine.h"
Transform::Transform(Shader* shader, string textureName, Shape shape, glm::vec3 position, glm::vec3 scale) {
	textures = GLEngine::tm->getTexture(textureName);
	this->shader = shader;
	if (textures == NULL) {
		//textures = tm->addTextures(textureName, texturePath);
		this->shader = GLEngine::tm->nullShader;
	}
	model = glm::mat4(1.0f);
	translateModel(position);
	scaleModel(scale);
	this->shape = shape;
}

void Transform::activateTexture() {
	if (textures == NULL)
		return;
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures->albedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures->normalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures->metallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, textures->roughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, textures->AOMap);
}

void Transform::renderModel(bool enableTexture, bool enableShader) {
	if (enableTexture)
		activateTexture();
	if (enableShader)
		this->shader->use();
	shader->setMat4("model", this->model);

	switch (shape)
	{
	case Shape::Cube:
		GLEngine::renderer->renderCube();
		break;
	case Shape::Quad:
		GLEngine::renderer->renderQuad();
		break;
	case Shape::Sphere:
		GLEngine::renderer->renderSphere();
		break;
	case Shape::Mesh:
		break;
	default:
		break;
	}
}

void Transform::translateModel(glm::vec3 vec) {
	this->model = glm::translate(this->model, vec);
}

void Transform::scaleModel(glm::vec3 vec) {
	this->model = glm::scale(this->model, vec);
}

void Transform::scaleModel(int ratio) {
	this->model = glm::scale(this->model, glm::vec3(ratio, ratio, ratio));
}