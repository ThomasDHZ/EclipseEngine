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

	void SetSpritePosition(float x, float y);
	void SetSpritePosition(glm::vec2 position);
	void SetSpriteDepth(float Depth);
	void SetSpriteRotation(float x, float y);
	void SetSpriteRotation(glm::vec2 rotation);
	void SetSpriteScale(float x, float y);
	void SetSpriteScale(glm::vec2 Scale);
	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);

	//float GetSpriteDepth();
	//glm::vec2 GetSpritePosition();
	//glm::vec2 GetSpriteRotation();
	//glm::vec2 GetSpriteScale();

	//float* GetSpriteDepthPtr();
	//glm::vec2* GetSpritePositionPtr();
	//glm::vec2* GetSpriteRotationPtr();
	//glm::vec2* GetSpriteScalePtr();

	//std::shared_ptr<Material> GetSpriteMaterial();
};

