#pragma once
#include "Mesh.h"
#include <GLTFModel.h>
#include <basetsd.h>

struct GLTFMeshLoader3D
{
	std::shared_ptr<GLTFNode> node;
	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int NodeID = -1;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);

	InstancingDataStruct instanceData;

	std::vector<MeshBoneWeights> BoneWeightList;
};

class Temp_GLTFMesh
{
private:
	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;

	std::string MeshName;
	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);

	std::shared_ptr<GLTFMaterial> gltfMaterial;
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
	VulkanBuffer MeshPropertiesBuffer;

	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> TransformMatrixBuffer;
	std::vector<VkDescriptorSet> descripterSetList;
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;

	void UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix);
	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);
	VkDescriptorBufferInfo UpdateMeshPropertiesBuffer();
	std::vector<VkDescriptorBufferInfo> UpdateMeshTransformBuffer();
	void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout, uint32_t descriptorsetIndex, uint32_t descriptorSetCount);
	void Destroy();

	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return TransformMatrixBuffer; }
};