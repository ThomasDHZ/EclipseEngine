#pragma once
#include <GLTFModel.h>
#include "Vertex.h"
#include "Mesh3D.h"
#include "Temp_GLTFMesh.h"
#include "GLTF_GraphicsDescriptors.h"

class GLTF_Temp_Model
{
private:
	static uint64_t ModelIDCounter;

	uint64_t ParentGameObjectID = 0;
	uint64_t ModelID = 0;

	std::string NodeName;
	//std::shared_ptr<GLTFNode> ParentMesh = nullptr;
	//std::vector<std::shared_ptr<GLTFNode>> ChildNodeList = std::vector<std::shared_ptr<GLTFNode>>();

	std::vector<GLTFPrimitive> PrimitiveList;
	std::vector<MeshProperties> MeshPropertiesList;
	std::vector<glm::mat4> TransformMatrixList;
	std::vector<std::shared_ptr<Texture2D>> TextureList;
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	std::vector<VulkanBuffer> MeshBufferList;
	std::vector<VulkanBuffer> MeshPropertiesBufferList;
	std::vector<VulkanBuffer> MeshTransformBufferList;

	std::vector<VkDescriptorBufferInfo> MeshPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> TransformMatrixBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	glm::mat4 GameObjectTransformMatrix;
	glm::mat4 ModelTransformMatrix;

	std::vector<std::shared_ptr<Temp_GLTFMesh>> MeshList;
	std::vector<VkDescriptorSet> descripterSetList;
	void GenerateID();
public:

	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	GLTF_Temp_Model();
	GLTF_Temp_Model(const std::string FilePath, glm::mat4 GameObjectMatrix, uint32_t gameObjectID);
	~GLTF_Temp_Model();

	void UpdateNodeTransform(std::shared_ptr<Temp_GLTFMesh> mesh, std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix);
	void Update(const glm::mat4& GameObjectTransformMatrix);
	void UpdateDescriptorSets();
	void UpdateMeshPropertiesBuffer();
	void UpdateMeshTransformBuffer();

	void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout);
	void Destroy();

	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return MeshPropertiesBuffer; };
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return TransformMatrixBuffer; };
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return TexturePropertiesBuffer; };
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return MaterialPropertiesBuffer; };
};
