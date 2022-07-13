#pragma once
#include "Mesh2D.h"
class LineMesh2D : public Mesh2D
{
private:
	std::vector<LineVertex2D> LineVertexList;
	std::vector<uint32_t> LineIndexList;

public:
	LineMesh2D();
	LineMesh2D(std::vector<LineVertex2D>& lineVertexList, uint64_t parentGameObjectID);
	LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID);
	LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID);
	~LineMesh2D();
};

