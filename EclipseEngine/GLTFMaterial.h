#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "Texture2D.h"

struct GLTFMaterialBufferInfo
{
	alignas(16) glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	alignas(4) float Metallic = 0.0f;
	alignas(4) float Roughness = 0.0f;
	alignas(4) float AmbientOcclusion = 1.0f;
	alignas(16) glm::vec3 Emission = glm::vec3(0.0f);
	alignas(4) float Alpha = 1.0f;

	alignas(4) uint32_t AlbedoMap = -1;
	alignas(4) uint32_t MetallicRoughnessMap = -1;
	alignas(4) uint32_t AmbientOcclusionMap = -1;
	alignas(4) uint32_t NormalMap = -1;
	alignas(4) uint32_t DepthMap = -1;
	alignas(4) uint32_t AlphaMap = -1;
	alignas(4) uint32_t EmissionMap = -1;
};

class GLTFMaterial
{
private:
	static uint64_t MaterialIDCounter;

	uint64_t MaterialID = 0;
	uint64_t MaterialBufferIndex = 0;

	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;
	GLTFMaterialBufferInfo MaterialInfo;
	std::string MaterialName;
	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicRoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;

	VkDescriptorSet descriptorSet;

	GLTFMaterial();
	GLTFMaterial(const std::string& materialName);
	~GLTFMaterial();

	void UpdateMaterialBufferIndex(uint64_t bufferIndex);

	void  GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& LightPropertiesBufferList)
	{
		VkDescriptorBufferInfo LightBufferInfo = {};
		LightBufferInfo.buffer = MaterialBuffer.Buffer;
		LightBufferInfo.offset = 0;
		LightBufferInfo.range = VK_WHOLE_SIZE;
		LightPropertiesBufferList.emplace_back(LightBufferInfo);
	}
	void UpdateBuffer()
	{
		MaterialBuffer.CopyBufferToMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
	}
	VkDescriptorImageInfo GetAlbedoMapDescriptor();
	VkDescriptorImageInfo GetMetallicRoughnessMapDescriptor();
	VkDescriptorImageInfo GetAmbientOcclusionMapDescriptor();
	VkDescriptorImageInfo GetNormalMapDescriptor();
	VkDescriptorImageInfo GetDepthMapDescriptor();
	VkDescriptorImageInfo GetAlphaMapDescriptor();
	VkDescriptorImageInfo GetEmissionMapDescriptor();
};