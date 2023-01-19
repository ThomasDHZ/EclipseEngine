#pragma once
#include "Mesh.h"
#include "MeshRendererManager.h"

struct MeshLoader3D
{
	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	std::vector<Vertex3D> VerticeList;
	std::vector<uint32_t> IndexList;
	std::shared_ptr<Material> MaterialPtr = MaterialManager::GetDefaultMaterial();
	InstancingDataStruct instanceData;

	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	uint32_t BoneCount = 0;
	std::vector<MeshBoneWeights> BoneWeightList;
};

class Mesh3D : public Mesh
{
protected:
	std::vector<Vertex3D> VertexList;
	std::vector<uint32_t> IndexList;

	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	glm::mat4 LastTransform = glm::mat4(1.0f);

	void InstancingStartUp(InstancingDataStruct& instanceData);
	void AnimationStartUp(MeshLoader3D& meshLoader);
	void RTXMeshStartUp();
	void UpdateMeshBottomLevelAccelerationStructure();

public:
	Mesh3D();
	Mesh3D(MeshLoader3D& meshLoader);
	~Mesh3D();

	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList) override;
	virtual void Destroy() override;

	void SetMeshPosition(float x, float y, float z);
	void SetMeshRotation(float x, float y, float z);
	void SetMeshScale(float x, float y, float z);
	void SetMeshPosition(glm::vec3 position);
	void SetMeshRotation(glm::vec3 rotation);
	void SetMeshScale(glm::vec3 scale);

	VulkanBuffer* GetTransformBufferPtr() { return &TransformBuffer; }
	VulkanBuffer* GetTransformInverseBufferPtr() { return &TransformInverseBuffer; }
	VulkanBuffer* GetBoneWeightBufferPtr() { return &BoneWeightBuffer; }
	VulkanBuffer* GetBoneTransformBufferPtr() { return &BoneTransformBuffer; }

	std::vector<Vertex3D> GetVertexList() { return VertexList; }
	uint32_t GetBoneCount() { return BoneCount; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	//	JsonConverter::to_json(json["MeshPosition"], MeshPosition);
	//	JsonConverter::to_json(json["MeshRotation"], MeshRotation);
	//	JsonConverter::to_json(json["MeshScale"], MeshScale);

	//	Mesh::to_json(json);
	//}
};

