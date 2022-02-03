#pragma once
#include "ComponentRenderer.h"
#include "Mesh.h"

class MeshRenderer : public ComponentRenderer
{
private:
	std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	std::vector<uint32_t> indices = {
	   0, 1, 2
	};

public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;
};

