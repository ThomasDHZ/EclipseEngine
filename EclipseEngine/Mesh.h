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
	std::vector<Vertex2D> VertexData;
	std::vector<uint32_t> IndiceData;
	glm::vec3 InstancePosition = glm::vec3(0.0f);
	glm::vec3 InstanceRotation = glm::vec3(0.0f);
	glm::vec3 InstanceScale = glm::vec3(1.0f);
	GLTFInstanceMeshDataStruct() {};
};

struct GLTFInstancingDataStruct
{
	std::vector<GLTFInstanceMeshDataStruct> InstanceMeshDataList;
	std::vector<glm::vec2> UVOffset;
	std::vector<std::shared_ptr<Material>> MaterialList;
	int PaletteRotation = 0;
	int PaletteIndex = 0;
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
	uint64_t MeshID = 0;
	uint64_t ParentModelID = 0;
	uint64_t ParentGameObjectID = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;
	uint32_t TriangleCount = 0;
	uint32_t InstanceCount = 0;

	uint64_t MeshBufferIndex = -1;
	uint32_t TransformIndex = 0;
	uint32_t ReflectionIndex = 0;

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

	std::string MeshName;

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

	void UpdateMeshBufferIndex(uint64_t meshBufferIndex, uint64_t transformBufferIndex);
	void UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix);
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList);

	template<class T>
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
		{
			for (auto& primitve : PrimitiveList)
			{
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer->Buffer, offsets);
				vkCmdBindIndexBuffer(commandBuffer, IndexBuffer->Buffer, 0, VK_INDEX_TYPE_UINT32);
				if (primitve.IndexCount > 0)
				{
					vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
					vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
				}
			}
		}
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer);
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, LightViewSceneData& constBuffer);

	template<class T>
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 1, 1, InstanceBuffer.GetBufferPtr(), offsets);
		if (IndexCount == 0)
		{
			vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
		}
		else
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, IndexCount, InstanceCount, 0, 0, 0);
		}
	}
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer);

	template<class T>
	void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer->Buffer, offsets);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer->Buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer);
	
	template<class T>
	void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 1, 1, InstanceBuffer.GetBufferPtr(), offsets);
		if (IndexCount == 0)
		{
			vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
		}
		else
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, IndexCount, InstanceCount, 0, 0, 0);
		}
	}
	virtual void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer);

	virtual void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout);
	virtual void Destroy();

	std::vector<std::shared_ptr<Material>> GetMaterialList() { return gltfMaterialList; }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return TransformMatrixBuffer; }
	uint64_t GetBLASBufferDeviceAddress() { return BottomLevelAccelerationBuffer.GetAccelerationBufferDeviceAddress(); }
};