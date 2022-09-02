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

struct MeshLoadingInfo
{
	uint64_t ModelID; 
	uint64_t GameObjectID;

	std::vector<Vertex3D> vertices;
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

	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t BoneCount = 0;
	uint32_t BufferIndex = 0;
	bool SelectedMesh = false;

	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	glm::vec3 ReflectionPoint = glm::vec3(0.0f);
	glm::mat4 MeshTransformMatrix = glm::mat4(1.0f);

	
	std::shared_ptr<Material> material;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer InstanceBuffer;
	MeshPropertiesUniformBuffer MeshPropertiesBuffer;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	void GenerateID();
	void GenerateColorID();
public:

	Mesh();
	Mesh(MeshTypeEnum meshType, MeshSubTypeEnum meshSubType, uint64_t ParentGameObjectID);
	~Mesh();

	std::string MeshName;
	MeshProperties meshProperties;

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	virtual void Destroy();

	void Draw(VkCommandBuffer& commandBuffer);
	void InstanceDraw(VkCommandBuffer& commandBuffer, VkBuffer* InstanceBuffer);
	void SetSelectedMesh(bool selected);
	void SetParentGameObjectID(uint64_t GameObjectID);
	void SetParentModel(uint64_t ModelID);
	void SetBufferIndex(int bufferIndex);
	void SetMaterial(std::shared_ptr<Material> materialPtr);
	void SetMaterial(const std::string& MaterialPath);
	void SetReflectionPoint(glm::vec3 reflectionPoint);

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

	virtual void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["MeshTransformMatrix"], MeshTransformMatrix);

		meshProperties.to_json(json["MeshProperties"]);
		json["MaterialPath"] = "../Materials/" + material->GetMaterialName() + ".txt";
	}
};

