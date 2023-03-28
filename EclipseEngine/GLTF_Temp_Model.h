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

	std::vector<GLTFPrimitive> PrimitiveList;
	std::vector<MeshProperties> MeshPropertiesList;
	std::vector<std::shared_ptr<Texture2D>> TextureList;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	std::vector<VulkanBuffer> MeshPropertiesBufferList;

	std::vector<VkDescriptorBufferInfo> MeshPropertiesBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> SunLightPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> DirectionalLightPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> PointLightPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> SpotLightPropertiesBuffer;

	glm::mat4 GameObjectTransformMatrix;
	glm::mat4 ModelTransformMatrix;



	std::vector<VkDescriptorSet> descripterSetList;
	void GenerateID();
public:
	std::vector<std::shared_ptr<Temp_GLTFMesh>> MeshList;
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;
	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	GLTF_Temp_Model();
	GLTF_Temp_Model(const std::string FilePath, glm::mat4 GameObjectMatrix, uint32_t gameObjectID);
	~GLTF_Temp_Model();

	void Update(const glm::mat4& GameObjectTransformMatrix);
	void UpdateMeshPropertiesBuffer();
	void UpdateSunLightPropertiesBuffer();
	void UpdateDirectionalLightPropertiesBuffer();
	void UpdatePointLightPropertiesBuffer();
	void UpdateSpotLightPropertiesBuffer();

	void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout, uint32_t descriptorsetIndex, uint32_t descriptorSetCount);
	void Destroy();

	std::vector<std::shared_ptr<GLTFSunLight>> SunLightList;
	std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	std::vector<std::shared_ptr<GLTFPointLight>> PointLightList;
	std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;

	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return MeshPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return MeshList[0]->TransformMatrixBuffer; }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return TexturePropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return MaterialPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetSunLightPropertiesBuffer() { return SunLightPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetDirectionalLightPropertiesBuffer() { return DirectionalLightPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetPointLightPropertiesBuffer() { return PointLightPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetSpotLightPropertiesBuffer() { return SpotLightPropertiesBuffer; }
};
