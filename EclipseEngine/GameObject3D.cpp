#include "GameObject3D.h"
#include "MeshRenderer.h"

GameObject3D::GameObject3D() 
{
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, position, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, position, rotation, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, position, rotation, scale, GameObjectID)));
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::SetGameObjectPosition(float x, float y, float z)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelPosition(glm::vec3(x, y, z));
}

void GameObject3D::SetGameObjectPosition(glm::vec3 position)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelPosition(position);
}

void GameObject3D::SetGameObjectRotation(float x, float y, float z)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelRotation(glm::vec3(x, y, z));
}

void GameObject3D::SetGameObjectRotation(glm::vec3 rotation)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelRotation(rotation);
}

void GameObject3D::SetGameObjectScale(float x, float y, float z)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelScale(glm::vec3(x, y, z));
}

void GameObject3D::SetGameObjectScale(glm::vec3 scale)
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	meshRenderer->GetModel()->SetModelScale(scale);
}

glm::vec3 GameObject3D::GetGameObjectPosition()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	return meshRenderer->GetModel()->GetModelPosition();
}

glm::vec3 GameObject3D::GetGameObjectRotation()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	return meshRenderer->GetModel()->GetModelRotation();
}

glm::vec3 GameObject3D::GetGameObjectScale()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	return meshRenderer->GetModel()->GetModelScale();
}

glm::vec3* GameObject3D::GetGameObjectPositionPtr()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	auto meshRendererRef = meshRenderer->GetModel()->GetModelPosition();
	return &meshRendererRef;
}

glm::vec3* GameObject3D::GetGameObjectRotationPtr()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	auto meshRendererRef = meshRenderer->GetModel()->GetModelRotation();
	return &meshRendererRef;
}

glm::vec3* GameObject3D::GetGameObjectScalePtr()
{
	const auto component = GetComponentByType(ComponentType::kMeshRenderer);
	const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
	auto meshRendererRef = meshRenderer->GetModel()->GetModelScale();
	return &meshRendererRef;
}