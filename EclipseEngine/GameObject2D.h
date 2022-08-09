#pragma once
#include "GameObject.h"

class GameObject2D : public GameObject
{
private:

public:
	GameObject2D();
	GameObject2D(const std::string Name);
	GameObject2D(const std::string Name, glm::vec2& position, uint32_t Depth);
	GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, uint32_t Depth);
	GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale, uint32_t Depth);
	GameObject2D(nlohmann::json& json);
	~GameObject2D();

	virtual void LoadPrefab(nlohmann::json& json) override;
	virtual void SaveAsPrefab(nlohmann::json& json) override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void SetGameObjectPosition(float x, float y);
	void SetGameObjectPosition(glm::vec2 position);
	void SetGameObjectDepth(float Depth);
	void SetGameObjectRotation(float x, float y);
	void SetGameObjectRotation(glm::vec2 rotation);
	void SetGameObjectScale(float x, float y);
	void SetGameObjectScale(glm::vec2 Scale);

	float GetGameObjectDepth() { return GameObject::GetGameObjectPosition().z; }
	glm::vec2 GetGameObjectPosition() { return glm::vec2(GameObject::GetGameObjectPosition().x, GameObject::GetGameObjectPosition().y); }
	glm::vec2 GetGameObjectRotation() { return glm::vec2(GameObject::GetGameObjectRotation().x, GameObject::GetGameObjectRotation().y); }
	glm::vec2 GetGameObjectScale() { return glm::vec2(GameObject::GetGameObjectScale().x, GameObject::GetGameObjectScale().y); }

	//virtual void to_json(nlohmann::json& json) override
	//{
	//	GameObject::to_json(json);
	//}
};

