#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform2D : public Component
{
private:


public:
	Transform2D();
	Transform2D(glm::vec2 position, uint32_t zIndex);
	Transform2D(glm::vec2 position, glm::vec2 rotation, uint32_t zIndex);
	Transform2D(glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex);

	virtual ~Transform2D();

	void Update(float DeltaTime) override;
	void Destroy() override;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::mat4 Transform = glm::mat4(1.0f);
	uint32_t ZIndex;
};

