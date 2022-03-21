#pragma once
#include "Component.h"
class Transform : public Component
{
public:
	glm::mat4 TransformMatrix = glm::mat4(1.0f);

	Transform();
	Transform(ComponentType componentType);
	~Transform();
};

