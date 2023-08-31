#pragma once
#include "GameObject2D.h"

class GridGameObject2D : public GameObject2D
{
public:
	GridGameObject2D();
	GridGameObject2D(const std::string Name, int drawLayer);
	GridGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer);
	GridGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	GridGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	~GridGameObject2D();

	void LoadGridGameObject2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, int drawLayer);
};
