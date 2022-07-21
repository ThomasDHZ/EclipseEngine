#pragma once
#include "GameObject.h"
#include "LineMesh3D.h"
#include "GameObjectManager.h"

class LineRenderer3D : public GameObject
{
private:
	std::shared_ptr<LineMesh3D> LineObject;

public:
	LineRenderer3D();

	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation);
	LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList);
	LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position);
	LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation);
	LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	~LineRenderer3D();

	virtual void Update(float DeltaTime) override;
};

