#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"

class Mesh
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	MeshPropertiesUniformBuffer meshProperties;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BufferIndex = 0;

public:


	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	~Mesh();

	void UpdateMeshProperties(MeshProperties& meshProperties);

	void Draw(VkCommandBuffer& commandBuffer);
	void Destory();


	void SetBufferIndex(int bufferIndex);

	VkBuffer GetMeshPropertiesBuffer() { return meshProperties.GetVulkanBufferData().GetBuffer(); }
};

