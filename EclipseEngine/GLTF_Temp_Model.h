#pragma once
#include <GLTFModel.h>
#include "Vertex.h"
#include "Mesh.h"

class GLTF_Temp_Model
{
private:
	static uint64_t ModelIDCounter;

	uint32_t VertexCount;
	uint32_t IndexCount;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer TransformBuffer;
	MeshPropertiesUniformBuffer MeshPropertiesUniformBuffer;
	MeshProperties meshProperties;

	std::vector<GLTFPrimitive> PrimitiveList;
	std::vector<std::shared_ptr<Texture2D>> TextureList;
	std::vector<std::shared_ptr<Material>> MaterialList;

	VkDescriptorBufferInfo MeshPropertiesBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	glm::mat4 TransformMatrix;
public:

	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	GLTF_Temp_Model();
	GLTF_Temp_Model(const std::string FilePath);
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
