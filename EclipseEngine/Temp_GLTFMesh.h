#pragma once
#include "Mesh.h"
#include <GLTFModel.h>
#include <basetsd.h>
#include "GLTFMaterial.h"

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
	std::vector<std::shared_ptr<GLTFMaterial>> gltfMaterialList;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	InstancingDataStruct InstanceData;

	std::vector<MeshBoneWeights> BoneWeightList;
};

class Temp_GLTFMesh
{
private:
	static uint64_t MeshIDCounter;

	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;
	uint64_t MeshBufferIndex = -1;

	std::string MeshName;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;
	uint32_t TriangleCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);

	std::vector<std::shared_ptr<GLTFMaterial>> gltfMaterialList;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void RTXMeshStartUp(VulkanBuffer& VertexBuffer, VulkanBuffer& IndexBuffer);
	void UpdateMeshBottomLevelAccelerationStructure();

public:
	Temp_GLTFMesh();
	Temp_GLTFMesh(GLTFMeshLoader3D& meshLoader);
	~Temp_GLTFMesh();

	std::shared_ptr<GLTFNode> ParentMesh = nullptr;
	std::vector<std::shared_ptr<GLTFNode>> ChildMeshList;
	std::vector<GLTFPrimitive> PrimitiveList;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	MeshProperties meshProperties;

	VulkanBuffer MeshTransformBuffer;
	VulkanBuffer MeshTransformInverseBuffer;
	VulkanBuffer MeshPropertiesBuffer;
	AccelerationStructureBuffer BottomLevelAccelerationBuffer;

	VkDescriptorBufferInfo UpdateMeshPropertiesBuffer();
	std::vector<VkDescriptorBufferInfo> UpdateMeshTransformBuffer();

	std::vector<VkDescriptorBufferInfo> TransformMatrixBuffer;
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;

	void UpdateMeshBufferIndex(uint64_t bufferIndex);
	void UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix);
	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	void Draw(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout ShaderPipelineLayout);
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void Destroy();

	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return TransformMatrixBuffer; }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
};