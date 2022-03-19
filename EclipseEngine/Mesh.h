#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"
#include "AccelerationStructureBuffer.h"
#include "Bone.h"

enum EnumMeshType
{
	kLine,
	kPolygon
};

struct MeshBoneWeights
{
	glm::ivec4 BoneID = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);
};

struct MeshLoadingInfo
{
	uint64_t ModelID; 
	std::vector<MeshVertex>& vertices;
	std::vector<uint32_t>& indices; 
	uint32_t BoneCount;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;
	glm::mat4 MeshTransform;
	std::shared_ptr<Material> materialPtr;

};

class Mesh
{
private:
	static uint64_t MeshIDCounter;

	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t BoneCount = 0;
	uint32_t BufferIndex = 0;

	EnumMeshType meshType;

	std::vector<MeshVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	std::shared_ptr<Material> material;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;
	MeshPropertiesUniformBuffer meshProperties;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void GenerateID();
	void MeshBottomLevelAccelerationStructure();

public:

	Mesh();
	Mesh(glm::vec3& StartPoint, glm::vec3& EndPoint);
	Mesh(std::vector<LineVertex>& vertices);
	Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	Mesh(MeshLoadingInfo meshLoader);
	~Mesh();

	void Draw(VkCommandBuffer& commandBuffer);
	void UpdateMeshProperties(MeshProperties& meshProps);
	void UpdateMeshProperties(MeshProperties& meshProps, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	void Destory();

	void SetParentModel(uint64_t ModelID);
	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);

	uint64_t GetMeshID() { return MeshID; }
	uint32_t GetMeshBufferIndex() { return BufferIndex; }
	VkBuffer GetMeshPropertiesBuffer() { return meshProperties.GetVulkanBufferData().GetBuffer(); }
	VkBuffer GetMeshVertexBuffer() { return VertexBuffer.GetBuffer(); }
	VkBuffer GetMeshIndiceBuffer() { return IndexBuffer.GetBuffer(); }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
	std::shared_ptr<Material> GetMaterial() { return material; }
	
	nlohmann::json ToJson()
	{
		nlohmann::json json;

		if (VertexList.size() != 0)
		{
			for (int x = 0; x < VertexList.size(); x++)
			{
				json["VertexList"][x] = VertexList[x].ToJson();
			}
		}
		if (IndexList.size() != 0)
		{
			json["IndexList"] = IndexList;
		}
		json["Material"] = material->ToJson();

		return json;
	}
};

