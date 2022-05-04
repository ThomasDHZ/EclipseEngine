#pragma once
#include "Vertex.h"
#include "Mesh.h"

class Skybox : public Mesh
{
private:
public:
	Skybox();
	~Skybox();

	void StartUp();
};
