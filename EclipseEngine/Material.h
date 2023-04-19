#pragma once
#include "JsonConverter.h"
#include "VulkanRenderer.h"
#include "TextureManager.h"
#include "VulkanBuffer.h"
#include <json.hpp>

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

	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["Ambient"], Ambient);
		JsonConverter::from_json(json["Diffuse"], Diffuse);
		JsonConverter::from_json(json["Specular"], Specular);
		JsonConverter::from_json(json["Albedo"], Albedo);
		JsonConverter::from_json(json["Metallic"], Metallic);
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
	}

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["Ambient"], Ambient);
		JsonConverter::to_json(json["Diffuse"], Diffuse);
		JsonConverter::to_json(json["Specular"], Specular);
		JsonConverter::to_json(json["Albedo"], Albedo);
		JsonConverter::to_json(json["Metallic"], Metallic);
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
	}
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
	float Matallic = 0.0f;
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

	static std::shared_ptr<Material> from_json(nlohmann::json json)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->GenerateID();

		JsonConverter::from_json(json["MaterialName"], material->MaterialName);
		material->MaterialType == json["MaterialType"].get_to(material->MaterialType);

		if (material->MaterialType == MaterialTypeEnum::kMaterialBlinnPhong)
		{
			JsonConverter::from_json(json["Ambient"], material->materialTextureData.Ambient);
			JsonConverter::from_json(json["Diffuse"], material->materialTextureData.Diffuse);
			JsonConverter::from_json(json["Specular"], material->materialTextureData.Specular);
			JsonConverter::from_json(json["Shininess"], material->materialTextureData.Shininess);
			JsonConverter::from_json(json["Reflectivness"], material->materialTextureData.Reflectivness);

			if (json.contains("DiffuseMap"))
			{
				material->DiffuseMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("DiffuseMap"))));
			}
			if (json.contains("SpecularMap"))
			{
				material->SpecularMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("SpecularMap"))));
			}
		}
		else
		{
			//JsonConverter::from_json(json["Albedo"], material->materialTextureData.Albedo);
			//JsonConverter::from_json(json["Metallic"], material->materialTextureData.Metallic);
			//JsonConverter::from_json(json["Roughness"], material->materialTextureData.Roughness);
			//JsonConverter::from_json(json["AmbientOcclusion"], material->materialTextureData.AmbientOcclusion);

			if (json.contains("AlbedoMap"))
			{
				material->AlbedoMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AlbedoMap"))));
			}
			if (json.contains("MetallicMap"))
			{
				material->MetallicMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("MetallicMap"))));
			}
			if (json.contains("RoughnessMap"))
			{
				material->RoughnessMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("RoughnessMap"))));
			}
			if (json.contains("AmbientOcclusionMap"))
			{
				material->AmbientOcclusionMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AmbientOcclusionMap"))));
			}
		}

		JsonConverter::from_json(json["Alpha"], material->materialTextureData.Alpha);
		if (json.contains("NormalMap"))
		{
			material->NormalMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("NormalMap"))));
		}
		if (json.contains("DepthMap"))
		{
			material->DepthMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("DepthMap"))));
		}
		if (json.contains("AlphaMap"))
		{
			material->AlphaMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AlphaMap"))));
		}
		if (json.contains("EmissionMap"))
		{
			material->EmissionMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("EmissionMap"))));
		}

		material->MaterialBuffer.CreateBuffer(&material->materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		material->MaterialBufferUpdate();
		VulkanRenderer::UpdateRendererFlag = true;

		return material;
	}

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["MaterialName"], MaterialName);
		JsonConverter::to_json(json["MaterialType"], MaterialType);
		if (MaterialType == MaterialTypeEnum::kMaterialBlinnPhong)
		{
			JsonConverter::to_json(json["Ambient"], Ambient);
			JsonConverter::to_json(json["Diffuse"], Diffuse);
			JsonConverter::to_json(json["Specular"], Specular);
			JsonConverter::to_json(json["Reflectivness"], Reflectivness);

			if (DiffuseMap != nullptr)
			{
				DiffuseMap->to_json(json["DiffuseMap"]);
			}
			if (SpecularMap != nullptr)
			{
				SpecularMap->to_json(json["SpecularMap"]);
			}
		}
		else
		{
			JsonConverter::to_json(json["Albedo"], Albedo);
			JsonConverter::to_json(json["Matallic"], Matallic);
			JsonConverter::to_json(json["Roughness"], Roughness);
			JsonConverter::to_json(json["AmbientOcclusion"], AmbientOcclusion);

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
		}

		JsonConverter::to_json(json["Alpha"], Alpha);

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
	}
};
