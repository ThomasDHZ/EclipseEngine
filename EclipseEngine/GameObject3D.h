#pragma once
#include "GameObject.h"

class GameObject3D : public GameObject
{
private:
public:
	GameObject3D();
	GameObject3D(const std::string Name, GameObjectRenderType renderType);
	GameObject3D(const std::string Name, GameObjectRenderType renderType, const glm::vec3& position);
	GameObject3D(const std::string Name, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation);
	GameObject3D(const std::string Name, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	GameObject3D(nlohmann::json& json);
	~GameObject3D();

	virtual void LoadPrefab(nlohmann::json& json) override;
	virtual void SaveAsPrefab(nlohmann::json& json) override;
	virtual void Update(float DeltaTime) override;

	void SetGameObjectPosition(float x, float y, float z);
	void SetGameObjectRotation(float x, float y, float z);
	void SetGameObjectScale(float x, float y, float z);
	void SetGameObjectPosition(const glm::vec3& gameObjectPosition);
	void SetGameObjectRotation(const glm::vec3& gameObjectRotation);
	void SetGameObjectScale(const glm::vec3& gameObjectScale);

	glm::mat4 GetGameObjectMatrix() { return GameObject::GetGameObjectMatrix(); }
	glm::vec3 GetGameObjectPosition() { return GameObject::GetGameObjectPosition(); }
	glm::vec3 GetGameObjectRotation() { return GameObject::GetGameObjectRotation(); }
	glm::vec3 GetGameObjectScale() { return GameObject::GetGameObjectScale(); }

	glm::vec3* GetGameObjectPositionPtr() { return &GameObjectPosition; }
	glm::vec3* GetGameObjectRotationPtr() { return &GameObjectRotation; }
	glm::vec3* GetGameObjectScalePtr() { return &GameObjectScale; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	//	GameObject::to_json(json);
	//}
};

