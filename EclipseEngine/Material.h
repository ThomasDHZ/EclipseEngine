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
	alignas(16) glm::vec3 CheckPaletteSwapColor = glm::vec3(0.0f);
	alignas(16) glm::vec3 PaletteSwapPixelColor = glm::vec3(0.0f);

	alignas(4) uint32_t AlbedoMap = -1;
	alignas(4) uint32_t MetallicRoughnessMap = -1;
	alignas(4) uint32_t MetallicMap = -1;
	alignas(4) uint32_t RoughnessMap = -1;
	alignas(4) uint32_t AmbientOcclusionMap = -1;
	alignas(4) uint32_t NormalMap = -1;
	alignas(4) uint32_t DepthMap = -1;
	alignas(4) uint32_t AlphaMap = -1;
	alignas(4) uint32_t EmissionMap = -1;
	alignas(4) uint32_t HeightMap = -1;
	alignas(4) uint32_t CheckPaletteSwapColorMap = -1;
	alignas(4) uint32_t PaletteSwapPixelColorMap = -1;
};

class Material
{
private:
	static uint64_t MaterialIDCounter;

	uint64_t MaterialID = 0;
	uint64_t MaterialBufferIndex = 0;

	VulkanBuffer MaterialBuffer;

	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicRoughnessMap = nullptr;
	std::shared_ptr<Texture> MetallicMap = nullptr;
	std::shared_ptr<Texture> RoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;
	std::shared_ptr<Texture> HeightMap = nullptr;
	std::shared_ptr<Texture> CheckPaletteSwapColorMap = nullptr;
	std::shared_ptr<Texture> PaletteSwapPixelColorMap = nullptr;

	void GenerateID();
	void UpdateBuffer();
public:
	std::string MaterialName;
	GLTFMaterialBufferInfo MaterialInfo;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;
	glm::vec3 CheckPaletteSwapColor = glm::vec3(0.0f);
	glm::vec3 PaletteSwapPixelColor = glm::vec3(0.0f);

	Material();
	Material(const std::string& materialName);
	~Material();

	void UpdateMaterialBufferIndex(uint64_t bufferIndex);
	
	void Destroy();
	void GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MaterialBufferList);

	void SetAlbedoMap(std::shared_ptr<Texture> texture);
	void SetMetallicRoughnessMap(std::shared_ptr<Texture> texture);
	void SetMetallicMap(std::shared_ptr<Texture> texture);
	void SetRoughnessMap(std::shared_ptr<Texture> texture);
	void SetAmbientOcclusionMap(std::shared_ptr<Texture> texture);
	void SetNormalMap(std::shared_ptr<Texture> texture);
	void SetDepthMap(std::shared_ptr<Texture> texture);
	void SetAlphaMap(std::shared_ptr<Texture> texture);
	void SetEmissionMap(std::shared_ptr<Texture> texture);
	void SetHeightMap(std::shared_ptr<Texture> texture);
	void SetCheckPaletteSwapColorMap(std::shared_ptr<Texture> texture);
	void SetPaletteSwapPixelColorMap(std::shared_ptr<Texture> texture);

	uint64_t GetMaterialBufferIndex() { return MaterialBufferIndex; }
	std::shared_ptr<Texture> GetAlbedoMap() { return AlbedoMap; }
	std::shared_ptr<Texture> GetMetallicRoughnessMap() { return MetallicRoughnessMap; }
	std::shared_ptr<Texture> GetMetallicMap() { return MetallicMap; }
	std::shared_ptr<Texture> GetRoughnessMap() { return RoughnessMap; }
	std::shared_ptr<Texture> GetAmbientOcclusionMap() { return AmbientOcclusionMap; }
	std::shared_ptr<Texture> GetNormalMap() { return NormalMap; }
	std::shared_ptr<Texture> GetDepthMap() { return DepthMap; }
	std::shared_ptr<Texture> GetAlphaMap() { return AlphaMap; }
	std::shared_ptr<Texture> GetEmissionMap() { return EmissionMap; }
	std::shared_ptr<Texture> GetHeightMap() { return HeightMap; }
	std::shared_ptr<Texture> GetCheckPaletteSwapColorMap() { return CheckPaletteSwapColorMap; }
	std::shared_ptr<Texture> GetPaletteSwapPixelColorMap() { return PaletteSwapPixelColorMap; }
};