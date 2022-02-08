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
	uint64_t GameObjectID;
	std::vector<std::shared_ptr<Component>> ComponentList;

	void FromJson(nlohmann::json& jsonArray)
	{
		for (int x = 0; x <= jsonArray.size(); x++)
		{
			if (x == 0)
			{
				jsonArray[0].at("ObjectName").get_to(ObjectName);
				jsonArray[0].at("GameObjectID").get_to(GameObjectID);
			}
			else
			{
				ComponentType type = ComponentType::kTransform2D;
				jsonArray[x].at("ComponentType").get_to(type);

				switch (type)
				{
				case ComponentType::kTransform2D:  { AddComponent(std::make_shared<Transform2D>(Transform2D(jsonArray[x]))); break; }
				case ComponentType::kTransform3D: { AddComponent(std::make_shared<Transform3D>(Transform3D(jsonArray[x]))); break; }
				}
			}
		}
	}

public:
	GameObject();
	GameObject(const std::string Name);
	GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec3 position);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	GameObject(nlohmann::json& json);
	virtual ~GameObject();

	virtual void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);
	void Destory();

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	void GenerateID();

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }

	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);

	std::vector<std::shared_ptr<Component>> GetComponentList() { return ComponentList; };

	void ToJson(nlohmann::json& json)
	{
		
		json = nlohmann::json::array();

		nlohmann::json gameObjectJson;
		gameObjectJson["ObjectName"] = ObjectName;
		gameObjectJson["GameObjectID"] = GameObjectID;
		json.emplace_back(gameObjectJson);
		
		for (auto component : ComponentList)
		{
			nlohmann::json componentJson;
			component->ToJson(componentJson);
			json.emplace_back(componentJson);
		}
	}
};

