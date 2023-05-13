#include "SpriteGameObject2D.h"

SpriteGameObject2D::SpriteGameObject2D()
{
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name) : GameObject2D(Name, kSpriteRenderer)
{
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer) : GameObject2D(Name, kSpriteRenderer, position, drawLayer)
{
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject2D(Name, kSpriteRenderer, position, rotation, drawLayer)
{
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, drawLayer)
{
}

SpriteGameObject2D::~SpriteGameObject2D()
{
}

void SpriteGameObject2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void SpriteGameObject2D::Destroy()
{
}

void SpriteGameObject2D::LoadSpriteGameObject2D(const std::string Name, std::shared_ptr<Material> material)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadSpriteMesh2D(Name, material, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

