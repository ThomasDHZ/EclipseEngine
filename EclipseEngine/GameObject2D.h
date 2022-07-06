#pragma once
#include "GameObject.h"

class GameObject2D : public GameObject
{
private:
public:
	GameObject2D();
	GameObject2D(const std::string Name, glm::vec2 position, uint32_t Depth);
	GameObject2D(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t Depth);
	GameObject2D(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t Depth);
	~GameObject2D();

	void SetPosition(float x, float y);
	void SetPosition(glm::vec2 position);
	void SetDepth(float Depth);
	void SetRotation(float x, float y);
	void SetRotation(glm::vec2 rotation);
	void SetScale(float x, float y);
	void SetScale(glm::vec2 Scale);
	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);

	float GetDepth();
	glm::vec2 GetPosition();
	glm::vec2 GetRotation();
	glm::vec2 GetScale();
	float* GetDepthPtr();
	glm::vec2* GetPositionPtr();
	glm::vec2* GetRotationPtr();
	glm::vec2* GetScalePtr();
	std::shared_ptr<Material> GetSpriteMaterial();
};

