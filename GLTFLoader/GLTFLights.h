#pragma once
#include "GLTFVertex.h"

struct GLTFSunLightLoader {
	std::string name;
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.00001f);
	float intensity = 1.0f;
};

struct GLTFDirectionalLightLoader {
	std::string name;
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.00001f);
	float intensity = 1.0f;
};

struct GLTFPointLightLoader {
	std::string name;
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.00001f);
	glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	float intensity = 1.0f;
	float radius = 1.0f;;
};

struct GLTFSpotLightLoader {
	std::string name;
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.00001f);
	glm::vec3 direction = glm::vec3(0.00001f);
	glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	float intensity = 1.0f;
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));
	float constant = 1.0f;
	float linear = 0.022f;
	float quadratic = 0.0019f;
};