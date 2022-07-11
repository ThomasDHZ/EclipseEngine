#pragma once
#include "Mesh2D.h"
class LineMesh2D : public Mesh2D
{
private:
public:
	LineMesh2D();
	LineMesh2D(std::vector<LineVertex>& vertices, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID);
	~LineMesh2D();
};

