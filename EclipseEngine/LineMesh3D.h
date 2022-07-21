#pragma once
#include "Mesh3D.h"
class LineMesh3D : public Mesh3D
{
private:

	std::vector<uint32_t> LineIndexList;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	void SetUpMesh();
	void BuildGrid3D(glm::vec3 GridSize, glm::vec3 GridSpacing);

public:
	LineMesh3D(std::vector<LineVertex3D>& vertices);
	LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint);
	LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& Color);
	LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color);
	LineMesh3D(int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ);
	~LineMesh3D();

	std::vector<LineVertex3D> LineVertexList;

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	void SetMeshPosition(float x, float y, float z);
	void SetMeshRotation(float x, float y, float z);
	void SetMeshScale(float x, float y, float z);
	void SetMeshPosition(glm::vec3 position);
	void SetMeshRotation(glm::vec3 rotation);
	void SetMeshScale(glm::vec3 scale);

	glm::vec3 GetPosition() { return MeshPosition; }
	glm::vec3 GetRotation() { return MeshRotation; }
	glm::vec3 GetScale() { return MeshScale; }
};

