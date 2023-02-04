#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "Texture2D.h"

class GLTFMaterial
{
private:
	static uint64_t MaterialIDCounter;

	uint64_t MaterialID = 0;
	uint64_t MaterialBufferIndex = 0;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;

	void GenerateID();

public:
	std::string MaterialName;
	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicRoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;

	GLTFMaterial();
	GLTFMaterial(const std::string& materialName);
	~GLTFMaterial();

	VkDescriptorImageInfo GetAlbedoMapDescriptor();
	VkDescriptorImageInfo GetMetallicRoughnessMapDescriptor();
	VkDescriptorImageInfo GetAmbientOcclusionMapDescriptor();
	VkDescriptorImageInfo GetNormalMapDescriptor();
	VkDescriptorImageInfo GetDepthMapDescriptor();
	VkDescriptorImageInfo GetAlphaMapDescriptor();
	VkDescriptorImageInfo GetEmissionMapDescriptor();
};