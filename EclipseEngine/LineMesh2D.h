#pragma once
#include "Mesh2D.h"
class LineMesh2D : public Mesh2D
{
private:

	void SetUpMesh();
public:
	LineMesh2D();
	LineMesh2D(std::vector<LineVertex2D>& lineVertexList, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& LineColor, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& LineColor, uint64_t parentGameObjectID);
	LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID);
	LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& LineColor, uint64_t parentGameObjectID);
	LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& LineColor , uint64_t parentGameObjectID);
	~LineMesh2D();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	std::vector<LineVertex2D> LineVertexList;
	std::vector<uint32_t> LineIndexList;
};

