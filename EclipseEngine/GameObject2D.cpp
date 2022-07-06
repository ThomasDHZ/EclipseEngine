#include "GameObject2D.h"
#include "SpriteRenderer.h"

GameObject2D::GameObject2D() : GameObject()
{
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2 position, uint32_t Depth) : GameObject(Name)
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, Depth), GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t Depth) : GameObject(Name)
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f), GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t Depth) : GameObject(Name)
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 0.0f), GameObjectID)));
}

GameObject2D::~GameObject2D()
{
}

//void GameObject2D::SetSpritePosition(float x, float y)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshPosition(glm::vec3(x, y, 0.0f));
//}
//
//void GameObject2D::SetSpritePosition(glm::vec2 position)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshPosition(glm::vec3(position.x, position.y, 0.0f));
//}
//
//void GameObject2D::SetSpriteDepth(float Depth)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshPosition(glm::vec3(0.0f, 0.0f, Depth));
//}
//
//void GameObject2D::SetSpriteRotation(float x, float y)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshRotation(glm::vec3(x, y, 0.0f));
//}
//
//void GameObject2D::SetSpriteRotation(glm::vec2 rotation)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshRotation(glm::vec3(rotation.x, rotation.y, 0.0f));
//}
//
//void GameObject2D::SetSpriteScale(float x, float y)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshScale(glm::vec3(x, y, 1.0f));
//}
//
//void GameObject2D::SetSpriteScale(glm::vec2 scale)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMeshScale(glm::vec3(scale.x, scale.y, 1.0f));
//}
//
//void GameObject2D::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	spriteRenderer->GetModel()->GetMeshList()[0]->SetMaterial(SpriteMaterial);
//}
//
//float GameObject2D::GetSpriteDepth()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	return spriteRenderer->GetModel()->GetMeshList()[0]->GetPosition().z;
//}
//
//glm::vec2 GameObject2D::GetSpritePosition()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	auto position = spriteRenderer->GetModel()->GetMeshList()[0]->GetPosition();
//	return glm::vec2(position.x, position.y);
//}
//
//glm::vec2 GameObject2D::GetSpriteRotation()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	auto rotation = spriteRenderer->GetModel()->GetMeshList()[0]->GetRotation();
//	return glm::vec2(rotation.x, rotation.y);
//}
//
//glm::vec2 GameObject2D::GetSpriteScale()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	auto scale = spriteRenderer->GetModel()->GetMeshList()[0]->GetScale();
//	return glm::vec2(scale.x, scale.y);
//}
//
//float* GameObject2D::GetSpriteDepthPtr()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	auto Depth = spriteRenderer->GetModel()->GetMeshList()[0]->GetPosition().z;
//	return &Depth;
//}
//
//glm::vec2* GameObject2D::GetSpritePositionPtr()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	const auto position = spriteRenderer->GetModel()->GetMeshList()[0]->GetPosition();
//	auto positionRef = glm::vec2(position.x, position.y);
//	return &positionRef;
//}
//
//glm::vec2* GameObject2D::GetSpriteRotationPtr()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	const auto rotation = spriteRenderer->GetModel()->GetMeshList()[0]->GetRotation();
//	auto rotationRef = glm::vec2(rotation.x, rotation.y);
//	return &rotationRef;
//}
//
//glm::vec2* GameObject2D::GetSpriteScalePtr()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	auto scale = spriteRenderer->GetModel()->GetMeshList()[0]->GetScale();
//	auto scaleRef = glm::vec2(scale.x, scale.y);
//	return &scaleRef;
//}
//
//std::shared_ptr<Material> GameObject2D::GetSpriteMaterial()
//{
//	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
//	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
//	return spriteRenderer->GetModel()->GetMeshList()[0]->GetMaterial();
//}
