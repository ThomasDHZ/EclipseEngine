#pragma once
#include "GameObject2D.h"

class SquareGameObject2D : public GameObject2D
{
public:
	SquareGameObject2D();
	SquareGameObject2D(const std::string Name, int drawLayer);
	SquareGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer);
	SquareGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	SquareGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	~SquareGameObject2D();

	void LoadSquareGameObject2D(const std::string Name, float size, const glm::vec3& Color, int drawLayer);
	void LoadSquareGameObject2D(const std::string Name, float size, const glm::vec4& Color, int drawLayer);
};