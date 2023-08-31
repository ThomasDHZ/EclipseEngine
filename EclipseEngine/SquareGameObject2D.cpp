#include "SquareGameObject2D.h"

SquareGameObject2D::SquareGameObject2D()
{
}

SquareGameObject2D::SquareGameObject2D(const std::string Name, int drawLayer) : GameObject2D(Name, kSquareRenderer2D)
{
}

SquareGameObject2D::SquareGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer) : GameObject2D(Name, kSquareRenderer2D, position, drawLayer)
{
}

SquareGameObject2D::SquareGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject2D(Name, kSquareRenderer2D, position, rotation, drawLayer)
{
}

SquareGameObject2D::SquareGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject2D(Name, kSquareRenderer2D, position, rotation, scale, drawLayer)
{
}

SquareGameObject2D::~SquareGameObject2D()
{
}

void SquareGameObject2D::LoadSquareGameObject2D(const std::string Name, float size, const glm::vec3& Color, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadSquareMesh2D(Name, size, Color, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void SquareGameObject2D::LoadSquareGameObject2D(const std::string Name, float size, const glm::vec4& Color, int drawLayer)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadSquareMesh2D(Name, size, Color, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}
