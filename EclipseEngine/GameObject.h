#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include "Mesh.h"

class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

	std::string ObjectName;
	uint64_t GameObjectID;
	std::vector<std::shared_ptr<Component>> ComponentList;

public:

	GameObject();
	GameObject(const std::string Name, std::vector<LineVertex>& VertexList, int a);
	GameObject(const std::string Name, glm::vec3 StartLine, glm::vec3 EndLine, int a);
	GameObject(const std::string Name);
	GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec3 position);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	GameObject(const std::string Name, const std::string filePath);
	GameObject(const std::string Name, const std::string filePath, glm::vec2 position);
	GameObject(const std::string Name, const std::string filePath, glm::vec2 position, glm::vec2 rotation);
	GameObject(const std::string Name, const std::string filePath, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale);
	GameObject(nlohmann::json& json);
	virtual ~GameObject();

	virtual void Update(float DeltaTime);
	void Destory();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);
	void TransferMesh(GameObject ToGameObject, std::shared_ptr<Mesh> mesh);

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	void GenerateID();

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }

	std::shared_ptr<Component> GetComponentBySubType(ComponentSubType componentType);
	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);

	std::vector<std::shared_ptr<Component>> GetComponentList() { return ComponentList; };

	bool operator==(const GameObject& rhs) const
	{ 
		return this->GameObjectID == rhs.GameObjectID; 
	}

	virtual void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["ObjectName"], ObjectName);

		for (int x = 0; x < ComponentList.size(); x++)
		{
			ComponentList[x]->to_json(json["ComponentList"][x]);
		}
	}
};

