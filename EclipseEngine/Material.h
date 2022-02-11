#pragma once
#include "VulkanRenderer.h"
#include "TextureManager.h"
#include "VulkanBuffer.h"

const uint32_t DefaultTextureID = 0;
const uint32_t DefaultAlphaTextureID = 1;

struct MaterialProperties
{
	glm::vec3 Ambient = glm::vec3(0.2f);
	glm::vec3 Diffuse = glm::vec3(0.6f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float Shininess = 32;
	float Reflectivness = 0.0f;

	std::shared_ptr<Texture> DiffuseMap = nullptr;
	std::shared_ptr<Texture> SpecularMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;
	std::shared_ptr<Texture> ShadowMap = nullptr;
};

struct MaterialBufferData
{
	alignas(16) glm::vec3 Ambient = glm::vec3(0.2f);
	alignas(16) glm::vec3 Diffuse = glm::vec3(0.6f);
	alignas(16) glm::vec3 Specular = glm::vec3(1.0f);
	alignas(4) float Shininess = 32;
	alignas(4) float Reflectivness = 0.0f;

	alignas(4) uint32_t DiffuseMapID = DefaultTextureID;
	alignas(4) uint32_t SpecularMapID = DefaultTextureID;
	alignas(4) uint32_t NormalMapID = DefaultTextureID;
	alignas(4) uint32_t DepthMapID = DefaultTextureID;
	alignas(4) uint32_t AlphaMapID = DefaultAlphaTextureID;
	alignas(4) uint32_t EmissionMapID = DefaultTextureID;
	alignas(4) uint32_t ShadowMapID = DefaultTextureID;
};

class Material
{
private:
    static uint64_t MaterialIDCounter;

	std::string MaterialName;
    uint64_t MaterialID = 0;
    uint64_t MaterialBufferIndex = 0;

	uint32_t DiffuseMapID = DefaultTextureID;
	uint32_t SpecularMapID = DefaultTextureID;
	uint32_t NormalMapID = DefaultTextureID;
	uint32_t DepthMapID = DefaultTextureID;
	uint32_t AlphaMapID = DefaultAlphaTextureID;
	uint32_t EmissionMapID = DefaultTextureID;
	uint32_t ShadowMapID = DefaultTextureID;

	MaterialProperties materialProperties;
	MaterialBufferData materialTextureData;
	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:

	Material();
	Material(const std::string materialName);
	Material(const std::string materialName, MaterialProperties& MaterialInfo);
	~Material();

	void Update(float DeltaTime);
	void Destroy();

	void LoadDiffuseMap(const std::string FilePath);
	void LoadSpecularMap(const std::string FilePath);
	void LoadNormalMap(const std::string FilePath);
	void LoadDepthMap(const std::string FilePath);
	void LoadAlphaMap(const std::string FilePath);
	void LoadEmissionMap(const std::string FilePath);

	void LoadDiffuseMap(uint64_t TextureIndex);
	void LoadSpecularMap(uint64_t TextureIndex);
	void LoadNormalMap(uint64_t TextureIndex);
	void LoadDepthMap(uint64_t TextureIndex);
	void LoadAlphaMap(uint64_t TextureIndex);
	void LoadEmissionMap(uint64_t TextureIndex);

	void LoadDiffuseMap(std::shared_ptr<Texture2D> texture);
	void LoadSpecularMap(std::shared_ptr<Texture2D> texture);
	void LoadNormalMap(std::shared_ptr<Texture2D> texture);
	void LoadDepthMap(std::shared_ptr<Texture2D> TextureIndex);
	void LoadAlphaMap(std::shared_ptr<Texture2D> TextureIndex);
	void LoadEmissionMap(std::shared_ptr<Texture2D> TextureIndex);

	void SetBufferIndex(int bufferIndex);

	std::string GetMaterialName() { return MaterialName; }
	uint64_t GetMaterialID() { return MaterialID; }
	uint64_t GetMaterialBufferIndex() { return MaterialBufferIndex; }
	MaterialBufferData GetMaterialTextureData() { return materialTextureData; }
	VkBuffer GetMaterialBuffer() { return MaterialBuffer.GetBuffer(); }
};

