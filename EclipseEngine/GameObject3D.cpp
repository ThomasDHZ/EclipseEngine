#include "GameObject3D.h"

GameObject3D::GameObject3D() 
{
}

GameObject3D::GameObject3D(const std::string Name) : GameObject(Name)
{
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position) : GameObject(Name, position)
{
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation) : GameObject(Name, position, rotation)
{
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name, position, rotation, scale)
{
}

GameObject3D::GameObject3D(nlohmann::json& json) : GameObject(json)
{
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::LoadPrefab(nlohmann::json& json)
{
	GameObject::LoadPrefab(json);
}

void GameObject3D::SaveAsPrefab(nlohmann::json& json)
{
	GameObject::SaveAsPrefab(json);
}

void GameObject3D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void GameObject3D::SetGameObjectPosition(float x, float y, float z)
{
	GameObject::SetGameObjectPosition(x, y, z);
}

void GameObject3D::SetGameObjectRotation(float x, float y, float z)
{
	GameObject::SetGameObjectRotation(x, y, z);
}

void GameObject3D::SetGameObjectScale(float x, float y, float z)
{
	GameObject::SetGameObjectScale(x, y, z);
}

void GameObject3D::SetGameObjectPosition(glm::vec3 gameObjectPosition)
{
	GameObject::SetGameObjectPosition(gameObjectPosition);
}

void GameObject3D::SetGameObjectRotation(glm::vec3 gameObjectRotation)
{
	GameObject::SetGameObjectRotation(gameObjectRotation);
}

void GameObject3D::SetGameObjectScale(glm::vec3 gameObjectScale)
{
	GameObject::SetGameObjectScale(gameObjectScale);
}