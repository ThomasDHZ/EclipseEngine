#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"

class Mesh
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;

public:
	MeshPropertiesUniformBuffer MeshProperties;
	uint32_t BufferIndex = 0;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	~Mesh();
	void Draw(VkCommandBuffer& commandBuffer);
};

