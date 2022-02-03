#pragma once
#include "Component.h"
#include "UniformBuffer.h"

class SpriteRenderer : public Component
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;


public:
	UniformBuffer<MeshProperties2>  MeshProperties;
	SpriteRenderer();
	virtual ~SpriteRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	void Draw(VkCommandBuffer& commandBuffer);
};

