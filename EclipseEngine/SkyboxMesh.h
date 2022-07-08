#pragma once
#include "Vertex.h"
#include "Mesh3D.h"

class SkyboxMesh : public Mesh3D
{
private:
public:
	SkyboxMesh();
	~SkyboxMesh();

	void StartUp();
};
