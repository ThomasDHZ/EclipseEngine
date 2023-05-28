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
	alignas(4) uint32_t MetallicMap = -1;
	alignas(4) uint32_t RoughnessMap = -1;
	alignas(4) uint32_t AmbientOcclusionMap = -1;
	alignas(4) uint32_t NormalMap = -1;
	alignas(4) uint32_t DepthMap = -1;
	alignas(4) uint32_t AlphaMap = -1;
	alignas(4) uint32_t EmissionMap = -1;
};

class Material
{
private:
	static uint64_t MaterialIDCounter;

	uint64_t MaterialID = 0;
	uint64_t MaterialBufferIndex = 0;

	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:
	std::string MaterialName;
	GLTFMaterialBufferInfo MaterialInfo;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;

	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicRoughnessMap = nullptr;
	std::shared_ptr<Texture> MetallicMap = nullptr;
	std::shared_ptr<Texture> RoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;

	Material();
	Material(const std::string& materialName);
	~Material();

	void UpdateMaterialBufferIndex(uint64_t bufferIndex);
	void UpdateBuffer();
	void Destroy();
	void GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MaterialBufferList);

	uint64_t GetMaterialBufferIndex() { return MaterialBufferIndex; }
};