#pragma once
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"
#include "AccelerationStructureBuffer.h"
#include "Bone.h"
#include "Converters.h"

enum MeshTypeEnum
{
	kUnkown,
	kPolygon,
	kLine
};

struct MeshBoneWeights
{
	glm::ivec4 BoneID = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);
};

struct MeshLoadingInfo
{
	uint64_t ModelID; 
	uint64_t GameObjectID;

	std::vector<MeshVertex> vertices;
	std::vector<uint32_t> indices; 
	uint32_t BoneCount;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;
	glm::mat4 MeshTransform;
	std::shared_ptr<Material> materialPtr;
	MeshTypeEnum meshType;

	MeshLoadingInfo() {}

};

class Mesh
{
private:
	static uint64_t MeshIDCounter;

	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t BoneCount = 0;
	uint32_t BufferIndex = 0;
	Pixel MeshColorID = NullPixel;

	MeshTypeEnum meshType;

	std::vector<MeshVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	glm::mat4 MeshTransformMatrix = glm::mat4(1.0f);

	MeshProperties meshProperties;
	std::shared_ptr<Material> material;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;
	MeshPropertiesUniformBuffer MeshPropertiesBuffer;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void GenerateID();
	void GenerateColorID();
	void MeshBottomLevelAccelerationStructure();

public:

	Mesh();
	Mesh(glm::vec3& StartPoint, glm::vec3& EndPoint);
	Mesh(std::vector<LineVertex>& vertices);
	Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	Mesh(MeshLoadingInfo& meshLoader);
	~Mesh();

	void Draw(VkCommandBuffer& commandBuffer);
	void Update();
	void Update(const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	void Destory();

	void SetParentGameObjectID(uint64_t GameObjectID);
	void SetParentModel(uint64_t ModelID);
	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);
	void SetMeshPosition(glm::vec3 position);
	void SetMeshRotation(glm::vec3 rotation);
	void SetMeshScale(glm::vec3 scale);

	void GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList);
	void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList);
	void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList);

	uint64_t GetMeshID() { return MeshID; }
	uint64_t GetParentModelID() { return ParentModelID; }
	uint64_t GetParentGameObjectID() 
	{ 
		return ParentGameObjectID; 
	}
	MeshTypeEnum GetMeshType() { return meshType; }
	Pixel GetMeshColorID() { return MeshColorID; }
	uint32_t GetMeshBufferIndex() { return BufferIndex; }
	VkBuffer GetMeshPropertiesBuffer() { return MeshPropertiesBuffer.GetVulkanBufferData().GetBuffer(); }
	VkBuffer GetMeshVertexBuffer() { return VertexBuffer.GetBuffer(); }
	VkBuffer GetMeshIndiceBuffer() { return IndexBuffer.GetBuffer(); }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
	glm::mat4 GetMeshTransformMatrix() { return MeshTransformMatrix; }

	std::shared_ptr<Material> GetMaterial() { return material; }
	glm::vec3* GetMeshPosition() { return &MeshPosition; }
	glm::vec3* GetMeshRotation() { return &MeshRotation; }
	glm::vec3* GetMeshScale() { return &MeshScale; };
	
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

	class MeshTypeSort
	{
	public:
		bool operator()(std::shared_ptr<Mesh> mesh1, std::shared_ptr<Mesh> mesh2)
		{
			return  mesh1->meshType < mesh2->meshType;
		}
	};

	class ZSorting
	{
	public:
		bool operator()(std::shared_ptr<Mesh> mesh1, std::shared_ptr<Mesh> mesh2)
		{
			return mesh1->MeshPosition.z < mesh2->MeshPosition.z;
		}
	};
};

