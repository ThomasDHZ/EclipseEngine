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

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Matallic = 0.5f;
	float Roughness = 0.5f;
	float AmbientOcclusion = 0.0f;
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
	std::shared_ptr<Texture> ShadowMap = nullptr;
};

struct MaterialBufferData
{
	alignas(16) glm::vec3 Ambient = glm::vec3(0.2f);
	alignas(16) glm::vec3 Diffuse = glm::vec3(0.6f);
	alignas(16) glm::vec3 Specular = glm::vec3(1.0f);
	alignas(4) float Shininess = 32;
	alignas(4) float Reflectivness = 0.0f;

	alignas(16) glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	alignas(4) float Matallic = 0.5f;
	alignas(4) float Roughness = 0.5f;
	alignas(4) float AmbientOcclusion = 0.0f;
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
	alignas(4) uint32_t ShadowMapID = DefaultTextureID;

	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["Ambient"], Ambient);
		JsonConverter::from_json(json["Diffuse"], Diffuse);
		JsonConverter::from_json(json["Specular"], Specular);
		JsonConverter::from_json(json["Albedo"], Albedo);
		JsonConverter::from_json(json["Matallic"], Matallic);
		JsonConverter::from_json(json["Roughness"], Roughness);
		JsonConverter::from_json(json["AmbientOcclusion"], AmbientOcclusion);
		JsonConverter::from_json(json["Alpha"], Alpha);
		JsonConverter::from_json(json["Shininess"], Shininess);
		JsonConverter::from_json(json["Reflectivness"], Reflectivness);

		JsonConverter::from_json(json["DiffuseMapID"], DiffuseMapID);
		JsonConverter::from_json(json["SpecularMapID"], SpecularMapID);
		JsonConverter::from_json(json["AlbedoMapID"], AlbedoMapID);
		JsonConverter::from_json(json["MetallicMapID"], MetallicMapID);
		JsonConverter::from_json(json["RoughnessMapID"], RoughnessMapID);
		JsonConverter::from_json(json["AmbientOcclusionMapID"], AmbientOcclusionMapID);
		JsonConverter::from_json(json["NormalMapID"], NormalMapID);
		JsonConverter::from_json(json["DepthMapID"], DepthMapID);
		JsonConverter::from_json(json["AlphaMapID"], AlphaMapID);
		JsonConverter::from_json(json["EmissionMapID"], EmissionMapID);
		JsonConverter::from_json(json["ShadowMapID"], ShadowMapID);
	}

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["Ambient"], Ambient);
		JsonConverter::to_json(json["Diffuse"], Diffuse);
		JsonConverter::to_json(json["Specular"], Specular);
		JsonConverter::to_json(json["Albedo"], Albedo);
		JsonConverter::to_json(json["Matallic"], Matallic);
		JsonConverter::to_json(json["Roughness"], Roughness);
		JsonConverter::to_json(json["AmbientOcclusion"], AmbientOcclusion);
		JsonConverter::to_json(json["Alpha"], Alpha);
		JsonConverter::to_json(json["Shininess"], Shininess);
		JsonConverter::to_json(json["Reflectivness"], Reflectivness);

		JsonConverter::to_json(json["DiffuseMapID"], DiffuseMapID);
		JsonConverter::to_json(json["SpecularMapID"], SpecularMapID);
		JsonConverter::to_json(json["AlbedoMapID"], AlbedoMapID);
		JsonConverter::to_json(json["MetallicMapID"], MetallicMapID);
		JsonConverter::to_json(json["RoughnessMapID"], RoughnessMapID);
		JsonConverter::to_json(json["AmbientOcclusionMapID"], AmbientOcclusionMapID);
		JsonConverter::to_json(json["NormalMapID"], NormalMapID);
		JsonConverter::to_json(json["DepthMapID"], DepthMapID);
		JsonConverter::to_json(json["AlphaMapID"], AlphaMapID);
		JsonConverter::to_json(json["EmissionMapID"], EmissionMapID);
		JsonConverter::to_json(json["ShadowMapID"], ShadowMapID);
	}
};

class Material
{
private:
    static uint64_t MaterialIDCounter;

	std::string MaterialName;
    uint64_t MaterialID = 0;
    uint64_t MaterialBufferIndex = 0;

	glm::vec3 Ambient = glm::vec3(0.2f);
	glm::vec3 Diffuse = glm::vec3(0.6f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float Shininess = 32;
	float Reflectivness = 0.0f;

	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Matallic = 0.5f;
	float Roughness = 0.5f;
	float AmbientOcclusion = 0.0f;
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
	std::shared_ptr<Texture> ShadowMap = nullptr;

	MaterialBufferData materialTextureData;
	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:

	Material();
	Material(const std::string materialName);
	Material(const std::string materialName, MaterialProperties& MaterialInfo);
	Material(nlohmann::json& json);
	~Material();

	void Update();
	void Destroy();

	void LoadDiffuseMap(const std::string FilePath);
	void LoadSpecularMap(const std::string FilePath);
	void LoadAlbedoMap(const std::string FilePath);
	void LoadMetallicMap(const std::string FilePath);
	void LoadRoughnessMap(const std::string FilePath);
	void LoadAmbientOcclusionMap(const std::string FilePath);
	void LoadNormalMap(const std::string FilePath);
	void LoadDepthMap(const std::string FilePath);
	void LoadAlphaMap(const std::string FilePath);
	void LoadEmissionMap(const std::string FilePath);

	void LoadDiffuseMap(uint64_t TextureIndex);
	void LoadSpecularMap(uint64_t TextureIndex);
	void LoadAlbedoMap(uint64_t TextureIndex);
	void LoadMetallicMap(uint64_t TextureIndex);
	void LoadRoughnessMap(uint64_t TextureIndex);
	void LoadAmbientOcclusionMap(uint64_t TextureIndex);
	void LoadNormalMap(uint64_t TextureIndex);
	void LoadDepthMap(uint64_t TextureIndex);
	void LoadAlphaMap(uint64_t TextureIndex);
	void LoadEmissionMap(uint64_t TextureIndex);

	void LoadDiffuseMap(std::shared_ptr<Texture2D> texture);
	void LoadSpecularMap(std::shared_ptr<Texture2D> texture);
	void LoadAlbedoMap(std::shared_ptr<Texture2D> texture);
	void LoadMetallicMap(std::shared_ptr<Texture2D> texture);
	void LoadRoughnessMap(std::shared_ptr<Texture2D> texture);
	void LoadAmbientOcclusionMap(std::shared_ptr<Texture2D> texture);
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
	std::shared_ptr<Texture> GetShadowMap() { return ShadowMap; }

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["MaterialName"], MaterialName);
		JsonConverter::to_json(json["Ambient"], Ambient);
		JsonConverter::to_json(json["Diffuse"], Diffuse);
		JsonConverter::to_json(json["Specular"], Specular);
		JsonConverter::to_json(json["Albedo"], Albedo);
		JsonConverter::to_json(json["Matallic"], Matallic);
		JsonConverter::to_json(json["Roughness"], Roughness);
		JsonConverter::to_json(json["AmbientOcclusion"], AmbientOcclusion);
		JsonConverter::to_json(json["Alpha"], Alpha);
		JsonConverter::to_json(json["Shininess"], Shininess);
		JsonConverter::to_json(json["Reflectivness"], Reflectivness);

		if (DiffuseMap != nullptr)
		{
			DiffuseMap->to_json(json["DiffuseMap"]);
		}
		if (SpecularMap != nullptr)
		{
			SpecularMap->to_json(json["SpecularMap"]);
		}
		if (AlbedoMap != nullptr)
		{
			AlbedoMap->to_json(json["AlbedoMap"]);
		}
		if (MetallicMap != nullptr)
		{
			MetallicMap->to_json(json["MetallicMap"]);
		}
		if (RoughnessMap != nullptr)
		{
			RoughnessMap->to_json(json["RoughnessMap"]);
		}
		if (AmbientOcclusionMap != nullptr)
		{
			AmbientOcclusionMap->to_json(json["AmbientOcclusionMap"]);
		}
		if (NormalMap != nullptr)
		{
			NormalMap->to_json(json["NormalMap"]);
		}
		if (DepthMap != nullptr)
		{
			DepthMap->to_json(json["DepthMap"]);
		}
		if (AlphaMap != nullptr)
		{
			AlphaMap->to_json(json["AlphaMap"]);
		}
		if (EmissionMap != nullptr)
		{
			EmissionMap->to_json(json["EmissionMap"]);
		}
		if (ShadowMap != nullptr)
		{
			ShadowMap->to_json(json["ShadowMap"]);
		}
	}
};

