#pragma once
#include "Vertex.h"
#include "Mesh3D.h"

class SkyboxMesh : public Mesh3D
{
private:
	std::vector<Vertex3D> VertexList;
	std::vector<uint32_t> IndexList;

public:
	SkyboxMesh();
	~SkyboxMesh();

	void StartUp();
};
