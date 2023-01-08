#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "Texture2D.h"

const uint32_t DefaultTextureID = 0;
const uint32_t DefaultAlphaTextureID = 1;

enum MaterialTypeEnum
{
	kMaterialnone,
	kMaterialBlinnPhong,
	kMaterialPBR
};

struct MaterialProperties
{
	MaterialTypeEnum materialType = kMaterialnone;
	glm::vec3 Ambient = glm::vec3(0.2f);
	glm::vec3 Diffuse = glm::vec3(0.6f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float Shininess = 32;
	float Reflectivness = 0.0f;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 0.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;

	std::shared_ptr<Texture> DiffuseMap = nullptr;
	std::shared_ptr<Texture> SpecularMap = nullptr;
	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicMap = nullptr;
	std::shared_ptr<Texture> RoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;
};

struct MaterialBufferData
{
	alignas(16) glm::vec3 Ambient = glm::vec3(0.2f);
	alignas(16) glm::vec3 Diffuse = glm::vec3(0.6f);
	alignas(16) glm::vec3 Specular = glm::vec3(1.0f);
	alignas(4) float Shininess = 32;
	alignas(4) float Reflectivness = 0.0f;

	alignas(16) glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	alignas(4) float Metallic = 0.0f;
	alignas(4) float Roughness = 0.0f;
	alignas(4) float AmbientOcclusion = 1.0f;
	alignas(16) glm::vec3 Emission = glm::vec3(0.0f);
	alignas(4) float Alpha = 1.0f;

	alignas(4) uint32_t DiffuseMapID = DefaultTextureID;
	alignas(4) uint32_t SpecularMapID = DefaultTextureID;
	alignas(4) uint32_t AlbedoMapID = DefaultTextureID;
	alignas(4) uint32_t MetallicMapID = DefaultTextureID;
	alignas(4) uint32_t RoughnessMapID = DefaultTextureID;
	alignas(4) uint32_t AmbientOcclusionMapID = DefaultTextureID;
	alignas(4) uint32_t NormalMapID = DefaultTextureID;
	alignas(4) uint32_t DepthMapID = DefaultTextureID;
	alignas(4) uint32_t AlphaMapID = DefaultAlphaTextureID;
	alignas(4) uint32_t EmissionMapID = DefaultTextureID;
};

class Material
{
private:
	static uint64_t MaterialIDCounter;
	MaterialTypeEnum MaterialType;

	std::string MaterialName;
	uint64_t MaterialID = 0;
	uint64_t MaterialBufferIndex = 0;

	glm::vec3 Ambient = glm::vec3(0.2f);
	glm::vec3 Diffuse = glm::vec3(0.6f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float Shininess = 32;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;

	std::shared_ptr<Texture> DiffuseMap = nullptr;
	std::shared_ptr<Texture> SpecularMap = nullptr;
	std::shared_ptr<Texture> AlbedoMap = nullptr;
	std::shared_ptr<Texture> MetallicMap = nullptr;
	std::shared_ptr<Texture> RoughnessMap = nullptr;
	std::shared_ptr<Texture> AmbientOcclusionMap = nullptr;
	std::shared_ptr<Texture> NormalMap = nullptr;
	std::shared_ptr<Texture> DepthMap = nullptr;
	std::shared_ptr<Texture> AlphaMap = nullptr;
	std::shared_ptr<Texture> EmissionMap = nullptr;

	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:
	float Reflectivness = 0.0f;
	Material();
	Material(const std::string materialName);
	Material(const std::string materialName, MaterialTypeEnum materialtype);
	Material(const std::string materialName, MaterialTypeEnum materialtype, MaterialProperties& MaterialInfo);
	~Material();


	MaterialBufferData materialTextureData;

	void MaterialBufferUpdate();
	void Destroy();

	void SetAlbedoMap(glm::vec4 albedo);
	void SetMetallicMap(float metallic);
	void SetRoughnessMap(float roughness);
	void SetAmbientOcclusionMap(float ambientOcclusion);
	void SetEmissionMap(glm::vec3 emission);

	void SetDiffuseMap(std::shared_ptr<Texture2D> texture);
	void SetSpecularMap(std::shared_ptr<Texture2D> texture);
	void SetAlbedoMap(std::shared_ptr<Texture2D> texture);
	void SetMetallicMap(std::shared_ptr<Texture2D> texture);
	void SetRoughnessMap(std::shared_ptr<Texture2D> texture);
	void SetAmbientOcclusionMap(std::shared_ptr<Texture2D> texture);
	void SetNormalMap(std::shared_ptr<Texture2D> texture);
	void SetDepthMap(std::shared_ptr<Texture2D> TextureIndex);
	void SetAlphaMap(std::shared_ptr<Texture2D> TextureIndex);
	void SetEmissionMap(std::shared_ptr<Texture2D> TextureIndex);

	void SetBufferIndex(int bufferIndex);

	std::string GetMaterialName() { return MaterialName; }
	MaterialTypeEnum GetMaterialType() { return MaterialType; }
	std::string* GetMaterialNamePtr() { return &MaterialName; }
	uint64_t GetMaterialID() { return MaterialID; }
	uint64_t GetMaterialBufferIndex() { return MaterialBufferIndex; }
	MaterialBufferData GetMaterialTextureData() { return materialTextureData; }
	VkBuffer GetMaterialBuffer() { return MaterialBuffer.GetBuffer(); }

	std::shared_ptr<Texture> GetDiffuseMap() { return DiffuseMap; }
	std::shared_ptr<Texture> GetSpecularMap() { return SpecularMap; }
	std::shared_ptr<Texture> GetAlbedoMap() { return AlbedoMap; }
	std::shared_ptr<Texture> GetMetallicMap() { return MetallicMap; }
	std::shared_ptr<Texture> GetRoughnessMap() { return RoughnessMap; }
	std::shared_ptr<Texture> GetAmbientOcclusionMap() { return AmbientOcclusionMap; }
	std::shared_ptr<Texture> GetNormalMap() { return NormalMap; }
	std::shared_ptr<Texture> GetDepthMap() { return DepthMap; }
	std::shared_ptr<Texture> GetAlphaMap() { return AlphaMap; }
	std::shared_ptr<Texture> GetEmissionMap() { return EmissionMap; }
};

