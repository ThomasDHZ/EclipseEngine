#pragma once
#include "Component.h"
#include "Mesh.h"

class SpriteRenderer : public Component
{
private:
	Mesh mesh;

public:
	SpriteRenderer();
	virtual ~SpriteRenderer();

	void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);
};

