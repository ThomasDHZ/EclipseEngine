#pragma once
#include "GameObject.h"

class GameObject3D : public GameObject
{
private:
public:
	GameObject3D();
	GameObject3D(const std::string Name, const std::string filePath);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~GameObject3D();


	void SetGameObjectPosition(float x, float y, float z);
	void SetGameObjectRotation(float x, float y, float z);
	void SetGameObjectScale(float x, float y, float z);
	void SetGameObjectPosition(glm::vec3 gameObjectPosition);
	void SetGameObjectRotation(glm::vec3 gameObjectRotation);
	void SetGameObjectScale(glm::vec3 gameObjectScale);

	glm::mat4 GetGameObjectMatrix() { return GameObject::GetGameObjectMatrix(); }
	glm::vec3 GetGameObjectPosition() { return GameObject::GetGameObjectPosition(); }
	glm::vec3 GetGameObjectRotation() { return GameObject::GetGameObjectRotation(); }
	glm::vec3 GetGameObjectScale() { return GameObject::GetGameObjectScale(); }
};

