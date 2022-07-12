#pragma once
#include "Mesh2D.h"
class LineMesh2D : public Mesh2D
{
private:
	std::vector<Vertex3D> VertexList;
	std::vector<uint32_t> IndexList;

public:
	LineMesh2D();
	LineMesh2D(std::vector<LineVertex3D>& vertices, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID);
	~LineMesh2D();
};

