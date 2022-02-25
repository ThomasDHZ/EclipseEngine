#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"
#include "AccelerationStructureBuffer.h"

class Mesh
{
private:
	static uint64_t MeshIDCounter;

	std::vector<Vertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::shared_ptr<Material> material;

	uint64_t MeshID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BufferIndex = 0;
	uint32_t PrimitiveCount = 0; //TriangleCount

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;
	MeshPropertiesUniformBuffer meshProperties;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void MeshBottomLevelAccelerationStructure();

public:

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	~Mesh();

	void GenerateID();
	void Draw(VkCommandBuffer& commandBuffer);
	void UpdateMeshProperties(MeshProperties& meshProperties);
	void Destory();

	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);

	uint64_t GetMeshID() { return MeshID; }
	uint32_t GetMeshBufferIndex() { return BufferIndex; }
	VkBuffer GetMeshPropertiesBuffer() { return meshProperties.GetVulkanBufferData().GetBuffer(); }
	VkBuffer GetMeshVertexBuffer() { return VertexBuffer.GetBuffer(); }
	VkBuffer GetMeshIndiceBuffer() { return IndexBuffer.GetBuffer(); }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
	std::shared_ptr<Material> GetMaterial() { return material; }
	
};

