#pragma once
#include "VulkanBuffer.h"
#include "Vertex.h"
class Mesh
{

private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
public:

	Mesh();
	Mesh(std::vector<Vertex>& VertexList, std::vector<uint32_t>& IndexList);
	~Mesh();

	void Update();
	void Draw(VkCommandBuffer& commandBuffer);
	void Destory();
};

