#pragma once
#include "Mesh.h"
#include <GLTFModel.h>
#include <basetsd.h>
#include "Material.h"
#include "Bone.h"
#include "UniformBuffer.h"
#include "AccelerationStructureBuffer.h"

struct GLTFInstanceMeshDataStruct
{
	uint64_t MaterialBufferIndex = 0;
	glm::vec3 InstancePosition = glm::vec3(0.0f);
	glm::vec3 InstanceRotation = glm::vec3(0.0f);
	glm::vec3 InstanceScale = glm::vec3(1.0f);
	GLTFInstanceMeshDataStruct() {};
};

struct GLTFInstancingDataStruct
{
	std::vector<GLTFInstanceMeshDataStruct> InstanceMeshDataList;
	std::vector<std::shared_ptr<Material>> MaterialList;
	GLTFInstancingDataStruct() {};
};

struct MeshBoneWeights
{
	glm::ivec4 BoneID = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);
};

struct GLTFMeshLoader3D
{
	std::shared_ptr<GLTFNode> node;

	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int NodeID = -1;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	std::vector<std::shared_ptr<Material>> gltfMaterialList;

	std::shared_ptr<VulkanBuffer> VertexBuffer;
	std::shared_ptr<VulkanBuffer> IndexBuffer;

	GLTFInstancingDataStruct InstanceData;
	std::vector<MeshBoneWeights> BoneWeightList;
};

class Mesh
{
private:
	static uint64_t MeshIDCounter;

protected:
	std::string MeshName;

	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;
	uint64_t MeshBufferIndex = -1;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t InstanceCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);

	MeshProperties meshProperties;

	std::shared_ptr<VulkanBuffer> VertexBuffer;
	std::shared_ptr<VulkanBuffer> IndexBuffer;
	VulkanBuffer MeshTransformBuffer;
	VulkanBuffer MeshTransformInverseBuffer;
	VulkanBuffer MeshPropertiesBuffer;
	VulkanBuffer InstanceBuffer;
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	std::vector<std::shared_ptr<Material>> gltfMaterialList;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;
	std::vector<GLTFInstancingDataStruct> InstanceData;
	std::vector<InstancedVertexData3D> InstancedVertexDataList;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void MeshStartUp(GLTFMeshLoader3D& meshLoader);
	void InstancingStartUp(GLTFInstancingDataStruct& instanceData);
	void RTXMeshStartUp(std::shared_ptr<VulkanBuffer> VertexBuffer, std::shared_ptr<VulkanBuffer> IndexBuffer);
	void AnimationStartUp(GLTFMeshLoader3D& meshLoader);
	void UpdateMeshBottomLevelAccelerationStructure();

public:
	Mesh();
	Mesh(GLTFMeshLoader3D& meshLoader);
	~Mesh();

	std::shared_ptr<GLTFNode> ParentMesh = nullptr;
	std::vector<std::shared_ptr<GLTFNode>> ChildMeshList;
	std::vector<GLTFPrimitive> PrimitiveList;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	VkDescriptorBufferInfo UpdateMeshPropertiesBuffer();
	std::vector<VkDescriptorBufferInfo> UpdateMeshTransformBuffer();
	std::vector<VkDescriptorBufferInfo> TransformMatrixBuffer;

	void UpdateMeshBufferIndex(uint64_t bufferIndex);
	void UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix);
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	void Draw(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout ShaderPipelineLayout);
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, uint32_t ReflectionIndex);
	void DrawReflectionMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, uint32_t ReflectionIndex);
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout);
	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout);
	virtual void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout);
	virtual void Destroy();

	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return TransformMatrixBuffer; }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
};