#pragma once
#include <GLTFModel.h>
#include "Vertex.h"
#include "Mesh3D.h"
#include "Temp_GLTFMesh.h"


class GLTF_Temp_Model
{
private:
	static uint64_t ModelIDCounter;

	uint64_t ParentGameObjectID = 0;
	uint64_t ModelID = 0;

	std::string NodeName;
	std::shared_ptr<GLTFNode> ParentMesh = nullptr;
	std::vector<std::shared_ptr<GLTFNode>> ChildNodeList = std::vector<std::shared_ptr<GLTFNode>>();

	std::vector<GLTFPrimitive> PrimitiveList;
	std::vector<std::shared_ptr<Texture2D>> TextureList;
	std::vector<std::shared_ptr<Material>> MaterialList;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	std::vector<VulkanBuffer> MeshTransformBufferList;

	MeshProperties meshProperties;
	VkDescriptorBufferInfo MeshPropertiesBuffer;

	MeshPropertiesUniformBuffer MeshPropertiesUniformBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	glm::mat4 GameObjectTransformMatrix;
	glm::mat4 ModelTransformMatrix;

	std::vector<std::shared_ptr<Temp_GLTFMesh>> MeshList;

	void GenerateID();
public:

	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	GLTF_Temp_Model();
	GLTF_Temp_Model(const std::string FilePath, glm::mat4 GameObjectMatrix, uint32_t gameObjectID);
	~GLTF_Temp_Model();

	void Update(const glm::mat4& GameObjectTransformMatrix);
	void UpdateMeshPropertiesBuffer();
	void UpdateTexturePropertiesBuffer();
	void UpdateMaterialPropertiesBuffer();

	void Draw(VkCommandBuffer& commandBuffer);
	void Destroy();

	VkDescriptorBufferInfo GetMeshPropertiesBuffer() { return MeshPropertiesBuffer; };
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return TexturePropertiesBuffer; };
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return MaterialPropertiesBuffer; };
};
