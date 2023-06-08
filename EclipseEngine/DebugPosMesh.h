#pragma once
#include "Mesh.h"

class DebugPosMesh : public Mesh
{
private:
public: 
	DebugPosMesh();
	DebugPosMesh(glm::vec3 pos);
	~DebugPosMesh();
};

