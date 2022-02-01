#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

class GameObject
{
private:
	std::string ObjectName;
	std::vector<std::shared_ptr<Component>> ComponentList;

public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	std::shared_ptr<Component> GetComponent(std::shared_ptr<Component> component);
	std::vector<std::shared_ptr<Component>> GetComponentList() { return ComponentList; };
};

