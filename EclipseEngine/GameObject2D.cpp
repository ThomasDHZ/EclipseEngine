#include "GameObject2D.h"
#include "SpriteRenderer.h"

GameObject2D::GameObject2D() : GameObject()
{
}

GameObject2D::GameObject2D(const std::string Name) : GameObject(Name)
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::~GameObject2D()
{
}

void GameObject2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);

	if (auto spriteRendererComponent = GetComponentByType(ComponentType::kSpriteRenderer))
	{
		auto spriteRenderer = static_cast<SpriteRenderer*>(spriteRendererComponent.get());
		spriteRenderer->Update(GameObjectTransform, DeltaTime);
	}
}

void GameObject2D::SetGameObjectPosition(float x, float y)
{
	GameObject::SetGameObjectPosition(x,y, GameObject::GetGameObjectPosition().z);
}

void GameObject2D::SetGameObjectPosition(glm::vec2 position)
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

void GameObject2D::SetGameObjectRotation(glm::vec2 rotation)
{
	GameObject::SetGameObjectRotation(rotation.x, rotation.y, 0.0f);
}

void GameObject2D::SetGameObjectScale(float x, float y)
{
	GameObject::SetGameObjectRotation(x, y, 1.0f);
}

void GameObject2D::SetGameObjectScale(glm::vec2 scale)
{
	GameObject::SetGameObjectRotation(scale.x, scale.y, 1.0f);
}

void GameObject2D::SetGameObjectMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
	spriteRenderer->SetSpriteMaterial(SpriteMaterial);
}
