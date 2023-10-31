#pragma once
#include "VulkanEngine.h"
#include "VulkanBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include "Texture.h"
#include "ForwardRenderingPipeline.h"
#include "SceneData.h"
#include "AccelerationStructure.h"
#include "MaterialManager.h"
#include "bone.h"

class Mesh
{
private:

	void MeshBottomLevelAccelerationStructure(VulkanEngine& engine);
	VkTransformMatrixKHR GLMToVkTransformMatrix(glm::mat4 matrix)
	{
		return VkTransformMatrixKHR
		{
			matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		};
	}

public:
		glm::mat4 MeshTransform;

		glm::vec3 MeshPosition = glm::vec3(0.0f);
		glm::vec3 MeshRotation = glm::vec3(0.0f);
		glm::vec3 MeshScale = glm::vec3(1.0f);

		Material material;
	
		VulkanBuffer IndexBuffer;
		VulkanBuffer VertexBuffer;
		VulkanBuffer TransformBuffer;
		VulkanBuffer TransformInverseBuffer;
		VulkanBuffer MaterialBuffer;
		AccelerationStructure BottomLevelAccelerationBuffer;

		std::vector<Vertex> VertexList;

		std::string MeshName;
		uint32_t MeshID = 0;
		uint32_t NodeID = 0;
		uint32_t BoneCount = 0;
		uint32_t VertexCount;
		uint32_t IndexCount;
		uint32_t PrimitiveCount; //TriangleCount
		uint32_t FirstIndex;     //primitiveOffset
		uint32_t VertexOffset;   //firstVertex

		bool ShowMesh = true;

		VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
		VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};
		VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};

	Mesh();
	Mesh(VulkanEngine& engine, std::vector<Vertex>& VertexList, std::vector<uint32_t>& IndexList, uint32_t meshID);
	Mesh(VulkanEngine& engine, std::vector<Vertex>& VertexList, std::vector<uint32_t>& IndexList, Material MeshMaterial, uint32_t meshID);
	~Mesh();

	void SetUpMesh(VulkanEngine& engine, std::vector<Vertex>& VertexList, std::vector<uint32_t>& IndexList);

	void Update(VulkanEngine& engine);
	void Update(VulkanEngine& engine, const std::vector<std::shared_ptr<Bone>>& BoneList, std::shared_ptr<SceneDataStruct> scenedata);
	void Draw(VkCommandBuffer commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline);
	void Destory(VulkanEngine& engine);
};