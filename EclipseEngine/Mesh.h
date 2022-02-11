#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"

class Mesh
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	MeshPropertiesUniformBuffer meshProperties;
	std::shared_ptr<Material> material;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BufferIndex = 0;

public:

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	~Mesh();

	void UpdateMeshProperties(MeshProperties& meshProperties);

	void Draw(VkCommandBuffer& commandBuffer);
	void Destory();


	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);

	VkBuffer GetMeshPropertiesBuffer() { return meshProperties.GetVulkanBufferData().GetBuffer(); }
	std::shared_ptr<Material> GetMaterial() { return material; }
};

