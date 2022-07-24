#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include "Mesh.h"

class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

protected:
	std::string ObjectName;
	uint64_t GameObjectID;
	std::vector<std::shared_ptr<Component>> ComponentList;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);

	void SetGameObjectPosition(float x, float y, float z);
	void SetGameObjectRotation(float x, float y, float z);
	void SetGameObjectScale(float x, float y, float z);
	void SetGameObjectPosition(glm::vec3 gameObjectPosition);
	void SetGameObjectRotation(glm::vec3 gameObjectRotation);
	void SetGameObjectScale(glm::vec3 gameObjectScale);

	glm::mat4 GetGameObjectMatrix() { return GameObjectTransform; }
	glm::vec3 GetGameObjectPosition() { return GameObjectPosition; }
	glm::vec3 GetGameObjectRotation() { return GameObjectRotation; }
	glm::vec3 GetGameObjectScale() { return GameObjectScale; }

	glm::vec3* GetGameObjectPositionPtr() { return &GameObjectPosition; }
	glm::vec3* GetGameObjectRotationPtr() { return &GameObjectRotation; }
	glm::vec3* GetGameObjectScalePtr() { return &GameObjectScale; }
public:

	GameObject();
	GameObject(const std::string Name);
	GameObject(const std::string Name, glm::vec3 Position);
	GameObject(const std::string Name, glm::vec3 Position, glm::vec3 Rotation);
	GameObject(const std::string Name, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);
	GameObject(nlohmann::json& json);
	virtual ~GameObject();

	glm::vec3 GameObjectPosition = glm::vec3(0.0f);
	glm::vec3 GameObjectRotation = glm::vec3(0.0f);
	glm::vec3 GameObjectScale = glm::vec3(1.0f);

	virtual void Update(float DeltaTime);
	virtual void Destroy();

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

