#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "Transform2D.h"
#include "Transform3D.h"
class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

	std::string ObjectName;
	std::vector<std::shared_ptr<Component>> ComponentList;
	uint64_t GameObjectID;

public:
	GameObject();
	GameObject(const std::string Name);
	GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec3 position);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	virtual ~GameObject();

	virtual void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	void GenerateID();

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }

	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);

	std::vector<std::shared_ptr<Component>> GetComponentList() { return ComponentList; };
};

