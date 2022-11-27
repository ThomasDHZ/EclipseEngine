#pragma once
#include <future>

#include "Vertex.h"
#include "VulkanRenderer.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"
#include "AccelerationStructureBuffer.h"
#include "Bone.h"
#include "Converters.h"
#include "JsonConverter.h"

enum MeshTypeEnum
{
	Unknown,
	kSprite,
	kPolygon,
	kPolygonInstanced,
	kLightDebug,
	kSkybox,
	kLine
};

enum MeshSubTypeEnum
{
	kNormal,
	kBillboard
};

struct MeshBoneWeights
{
	glm::ivec4 BoneID = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);

	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["BoneID"], BoneID);
		JsonConverter::from_json(json["BoneWeights"], BoneWeights);
	}

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["BoneID"], BoneID);
		JsonConverter::to_json(json["BoneWeights"], BoneWeights);
	}
};

struct InstanceMeshDataStruct
{
	uint64_t MaterialBufferIndex = 0;
	glm::vec3 InstancePosition = glm::vec3(0.0f);
	glm::vec3 InstanceRotation = glm::vec3(0.0f);
	glm::vec3 InstanceScale = glm::vec3(1.0f);
	InstanceMeshDataStruct() {};
};

struct InstancingDataStruct
{
	std::vector<InstanceMeshDataStruct> instanceMeshDataList;
	std::vector<std::shared_ptr<Material>> MaterialList;
	InstancingDataStruct() {};
};

class Mesh
{
private:
	static uint64_t MeshIDCounter;

	Pixel MeshColorID = NullPixel;

	//void from_json(nlohmann::json& json)
	//{
	//	JsonConverter::from_json(json["MeshPosition"], MeshPosition);
	//	JsonConverter::from_json(json["MeshRotation"], MeshRotation);
	//	JsonConverter::from_json(json["MeshScale"], MeshScale);
	//	JsonConverter::from_json(json["MeshTransformMatrix"], MeshTransformMatrix);

	//	meshProperties.from_json(json["MeshProperties"]);

	//	std::string FilePath = "";
	//	json["MaterialPath"].get_to(FilePath);

	//	MaterialManager::LoadMaterial(FilePath);
	//}

protected:
	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	uint64_t MeshID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t InstanceCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t BoneCount = 0;
	uint32_t BufferIndex = 0;
	bool SelectedMesh = false;

	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	glm::mat4 GameObjectTransformMatrix = glm::mat4(1.0f);
	glm::mat4 ModelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 MeshTransformMatrix = glm::mat4(1.0f);
	std::vector<glm::mat4> BoneTransform;
	std::vector<MeshBoneWeights> BoneWeightList;

	glm::vec3 ReflectionPoint = glm::vec3(0.0f);

	std::vector<InstancingDataStruct> InstanceData;
	std::vector<InstancedData3D> InstancedDataList;
	
	std::shared_ptr<Material> material;



	void GenerateID();
	void GenerateColorID();

public:

	Mesh();
	Mesh(MeshTypeEnum meshType, MeshSubTypeEnum meshSubType, uint64_t ParentGameObjectID);
	~Mesh();

	std::string MeshName;
	MeshProperties meshProperties;
	glm::vec3 reflectionPoint = glm::vec3(0.0f);

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer InstanceBuffer;
	MeshPropertiesUniformBuffer MeshPropertiesBuffer;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	virtual void Destroy();

	void Draw(VkCommandBuffer& commandBuffer);
	void InstanceDraw(VkCommandBuffer& commandBuffer);
	void SetSelectedMesh(bool selected);
	void SetParentGameObjectID(uint64_t GameObjectID);
	void SetParentModel(uint64_t ModelID);
	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);
	void SetMaterial(const std::string& MaterialPath);
	void SetReflectionPoint(const glm::vec3& reflectionPoint);

	void GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList);
	void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList);
	void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList);

	uint64_t GetMeshID() { return MeshID; }
	uint64_t GetParentModelID() { return ParentModelID; }
	uint64_t GetParentGameObjectID() { return ParentGameObjectID; }
	MeshTypeEnum GetMeshType() { return MeshType; }
	MeshSubTypeEnum GetMeshSubType() { return MeshSubType; }
	Pixel GetMeshColorID() { return MeshColorID; }
	uint32_t GetMeshBufferIndex() { return BufferIndex; }
	VkBuffer GetMeshPropertiesBuffer() { return MeshPropertiesBuffer.GetVulkanBufferData().GetBuffer(); }
	VkBuffer GetMeshVertexBuffer() { return VertexBuffer.GetBuffer(); }
	VkBuffer* GetMeshVertexBufferPtr() { return VertexBuffer.GetBufferPtr(); }
	VkBuffer GetMeshIndiceBuffer() { return IndexBuffer.GetBuffer(); }
	VkBuffer GetInstanceBuffer() { return InstanceBuffer.GetBuffer(); }
	VkBuffer* GetInstanceBufferPtr() { return InstanceBuffer.GetBufferPtr(); }
	VulkanBuffer GetVertexVulkanBuffer() { return VertexBuffer; }
	VulkanBuffer* GetVertexVulkanBufferPtr() { return &VertexBuffer; }
	VulkanBuffer GetIndexVulkanBuffer() { return IndexBuffer; }
	VulkanBuffer* GetIndexVulkanBufferPtr() { return &IndexBuffer; }
	VulkanBuffer GetInstanceVulkanBuffer() { return InstanceBuffer; }
	VulkanBuffer* GetInstanceVulkanBufferPtr() { return &InstanceBuffer; }
	glm::mat4 GetMeshTransformMatrix() { return MeshTransformMatrix; }
	glm::vec3 GetReflectionPoint() { return ReflectionPoint; }

	std::shared_ptr<Material> GetMaterial() { return material; }
	MeshProperties GetMeshProperties() { return meshProperties; }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }

	glm::vec2* GetUVOffset() { return &meshProperties.UVOffset; }
	glm::vec2* GetUVScale() { return &meshProperties.UVScale; }
	glm::vec2* GetUVFlip() { return &meshProperties.UVFlip; }

	//class MeshTypeSort
	//{
	//public:
	//	bool operator()(std::shared_ptr<Mesh> mesh1, std::shared_ptr<Mesh> mesh2)
	//	{
	//		return  mesh1->MeshType < mesh2->MeshType;
	//	}
	//};

	//class ZSorting
	//{
	//public:
	//	bool operator()(std::shared_ptr<Mesh> mesh1, std::shared_ptr<Mesh> mesh2)
	//	{
	//		return mesh1->MeshPosition.z < mesh2->MeshPosition.z;
	//	}
	//};

	/*virtual void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["MeshTransformMatrix"], MeshTransformMatrix);

		meshProperties.to_json(json["MeshProperties"]);
		json["MaterialPath"] = "../Materials/" + material->GetMaterialName() + ".txt";
	}*/
};

