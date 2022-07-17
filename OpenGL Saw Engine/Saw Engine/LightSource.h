#pragma once
#include "glm\vec3.hpp"
class LightSource {
public:
	glm::vec3 position;
	glm::vec3 color;
	LightSource() {}
	LightSource(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}
};