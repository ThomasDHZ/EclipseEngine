#pragma once
#include "GameObject.h"

class GameObject3D : public GameObject
{
private:
public:
	GameObject3D();
	GameObject3D(const std::string Name, glm::vec3 position);
	GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	GameObject3D(const std::string Name, const std::string filePath);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~GameObject3D();
};

