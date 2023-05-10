#include "SpriteGameObject3D.h"

SpriteGameObject3D::SpriteGameObject3D()
{
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name) : GameObject3D(Name, kSpriteRenderer)
{
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position) : GameObject3D(Name, kSpriteRenderer, position)
{
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kSpriteRenderer, position, rotation)
{
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kSpriteRenderer, position, rotation, scale)
{
}

SpriteGameObject3D::~SpriteGameObject3D()
{
}

void SpriteGameObject3D::LoadSpriteGameObject3D(const std::string Name, std::shared_ptr<Material> material)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadSpriteMesh3D(Name, material, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}
