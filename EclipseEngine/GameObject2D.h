#pragma once
#include "GameObject.h"

class GameObject2D : public GameObject
{
private:

public:
	GameObject2D();
	GameObject2D(const std::string Name, GameObjectRenderType renderType);
	GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, int drawLayer);
	GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	GameObject2D(const std::string Name, GameObjectRenderType renderType, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	~GameObject2D();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void SetGameObjectPosition(float x, float y);
	void SetGameObjectPosition(const glm::vec2& position);
	void SetGameObjectDepth(float Depth);
	void SetGameObjectRotation(float x, float y);
	void SetGameObjectRotation(const glm::vec2& rotation);
	void SetGameObjectScale(float x, float y);
	void SetGameObjectScale(const glm::vec2& Scale);

	float GetGameObjectDepth() { return GameObject::GetGameObjectPosition().z; }
	glm::vec2 GetGameObjectPosition() { return glm::vec2(GameObject::GetGameObjectPosition().x, GameObject::GetGameObjectPosition().y); }
	glm::vec2 GetGameObjectRotation() { return glm::vec2(GameObject::GetGameObjectRotation().x, GameObject::GetGameObjectRotation().y); }
	glm::vec2 GetGameObjectScale() { return glm::vec2(GameObject::GetGameObjectScale().x, GameObject::GetGameObjectScale().y); }

	void LoadSpriteGameObject2D(const std::string Name, std::shared_ptr<Material> material);
};

