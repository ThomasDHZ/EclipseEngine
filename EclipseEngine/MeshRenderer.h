#pragma once
#include "Component.h"
#include "UniformBuffer.h"

class MeshRenderer : public Component
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;

	UniformBuffer<MeshProperties2>  MeshProperties;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	void Draw(VkCommandBuffer& commandBuffer);
};

