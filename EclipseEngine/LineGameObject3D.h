#pragma once
#include "GameObject3D.h"

class LineGameObject3D : public GameObject3D
{
public:
	LineGameObject3D();
	LineGameObject3D(const std::string Name);
	LineGameObject3D(const std::string Name, const glm::vec3& position);
	LineGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation);
	LineGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	~LineGameObject3D();

	void LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint);
	void LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color);
	void LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor);
	void LoadLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList);
};

