#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
private:
	Mesh mesh;

public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);
};

