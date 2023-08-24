#include "LineGameObject2D.h"

LineGameObject2D::LineGameObject2D()
{
}

LineGameObject2D::LineGameObject2D(const std::string Name, int drawLayer) : GameObject2D(Name, kLineRenderer2D)
{
}

LineGameObject2D::LineGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer) : GameObject2D(Name, kLineRenderer2D, position, drawLayer)
{
}

LineGameObject2D::LineGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject2D(Name, kLineRenderer2D, position, rotation, drawLayer)
{
}

LineGameObject2D::LineGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject2D(Name, kLineRenderer2D, position, rotation, scale, drawLayer)
{
}

LineGameObject2D::~LineGameObject2D()
{
}

void LineGameObject2D::LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh2D(Name, StartPoint, EndPoint, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject2D::LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh2D(Name, StartPoint, EndPoint, Color, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject2D::LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh2D(Name, StartPoint, EndPoint, StartColor, EndColor, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject2D::LoadLineGameObject2D(const std::string Name, std::vector<LineVertex2D> VertexList, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh2D(Name, VertexList, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject2D::LoadGridGameObject2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadGridMesh2D(GridName, GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}
