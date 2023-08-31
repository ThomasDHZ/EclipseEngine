#include "GridGameObject2D.h"

GridGameObject2D::GridGameObject2D()
{
}

GridGameObject2D::GridGameObject2D(const std::string Name, int drawLayer) : GameObject2D(Name, kGridRenderer2D)
{
}

GridGameObject2D::GridGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer) : GameObject2D(Name, kGridRenderer2D, position, drawLayer)
{
}

GridGameObject2D::GridGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject2D(Name, kGridRenderer2D, position, rotation, drawLayer)
{
}

GridGameObject2D::GridGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject2D(Name, kGridRenderer2D, position, rotation, scale, drawLayer)
{
}

GridGameObject2D::~GridGameObject2D()
{
}

void GridGameObject2D::LoadGridGameObject2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadGridMesh2D(GridName, GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}
