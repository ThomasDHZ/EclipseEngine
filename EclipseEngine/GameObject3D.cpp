#include "GameObject3D.h"
#include "MeshRenderer.h"

GameObject3D::GameObject3D() 
{
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, rotation, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, rotation, scale, GameObjectID)));
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
