#include "GameObject2D.h"

GameObject2D::GameObject2D() : GameObject()
{
}

GameObject2D::GameObject2D(const std::string Name, GameObjectRenderType renderType) : GameObject(Name, renderType)
{
	DrawLayer = 0;
}

GameObject2D::GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, int drawLayer) : GameObject(Name, renderType, glm::vec3(position.x, position.y, drawLayer))
{
	DrawLayer = drawLayer;
}

GameObject2D::GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject(Name, renderType, glm::vec3(position.x, position.y, drawLayer), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	DrawLayer = drawLayer;
}

GameObject2D::GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject(Name, renderType, glm::vec3(position.x, position.y, drawLayer), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	DrawLayer = drawLayer;
}

GameObject2D::~GameObject2D()
{
}

void GameObject2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void GameObject2D::Destroy()
{
}

void GameObject2D::SetGameObjectPosition(float x, float y)
{
	GameObject::SetGameObjectPosition(x,y, GameObject::GetGameObjectPosition().z);
}

void GameObject2D::SetGameObjectPosition(const glm::vec2& position)
{
	GameObject::SetGameObjectPosition(position.x, position.y, GameObject::GetGameObjectPosition().z);
}

void GameObject2D::SetGameObjectDepth(float Depth)
{
	GameObject::SetGameObjectPosition(GetGameObjectPosition().x, GameObject::GetGameObjectPosition().y, Depth);
}

void GameObject2D::SetGameObjectRotation(float x, float y)
{
	GameObject::SetGameObjectRotation(x, y, 0.0f);
}

void GameObject2D::SetGameObjectRotation(const glm::vec2& rotation)
{
	GameObject::SetGameObjectRotation(rotation.x, rotation.y, 0.0f);
}

void GameObject2D::SetGameObjectScale(float x, float y)
{
	GameObject::SetGameObjectRotation(x, y, 1.0f);
}

void GameObject2D::SetGameObjectScale(const glm::vec2& scale)
{
	GameObject::SetGameObjectRotation(scale.x, scale.y, 1.0f);
}

void GameObject2D::LoadSpriteGameObject2D(const std::string Name, std::shared_ptr<Material> material)
{
}
