#pragma once
#include "GameObject3D.h"

class SpriteGameObject3D : public GameObject3D
{
public:
	SpriteGameObject3D();
	SpriteGameObject3D(const std::string Name);
	SpriteGameObject3D(const std::string Name, const glm::vec3& position);
	SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation);
	SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	virtual ~SpriteGameObject3D();

	void LoadSpriteGameObject3D(const std::string Name, std::shared_ptr<Material> material);
};