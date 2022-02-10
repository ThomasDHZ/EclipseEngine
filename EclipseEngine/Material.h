#pragma once
#include "VulkanRenderer.h"
#include "Texture.h"
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

	void operator=(const MaterialProperties& materialTexture)
	{
		Ambient = materialTexture.Ambient;
		Diffuse = materialTexture.Diffuse;
		Specular = materialTexture.Specular;
		Shininess = materialTexture.Shininess;
		Reflectivness = materialTexture.Reflectivness;

		if (materialTexture.DiffuseMap != nullptr)
		{
			DiffuseMapID = materialTexture.DiffuseMap->GetTextureBufferIndex();
		}
		if (materialTexture.SpecularMap != nullptr)
		{
			SpecularMapID = materialTexture.SpecularMap->GetTextureBufferIndex();
		}
		if (materialTexture.NormalMap != nullptr)
		{
			NormalMapID = materialTexture.NormalMap->GetTextureBufferIndex();
		}
		if (materialTexture.DepthMap != nullptr)
		{
			DepthMapID = materialTexture.DepthMap->GetTextureBufferIndex();
		}
		if (materialTexture.AlphaMap != nullptr)
		{
			AlphaMapID = materialTexture.AlphaMap->GetTextureBufferIndex();
		}
		if (materialTexture.EmissionMap != nullptr)
		{
			EmissionMapID = materialTexture.EmissionMap->GetTextureBufferIndex();
		}
		if (materialTexture.ShadowMap != nullptr)
		{
			ShadowMapID = materialTexture.ShadowMap->GetTextureBufferIndex();
		}
	}
};

class Material
{
private:
    static uint64_t MaterialIDCounter;

	std::string MaterialName;
    uint64_t MaterialID = 0;
    uint64_t MaterialBufferIndex = 0;

	MaterialProperties materialProperties;
	MaterialBufferData materialTextureData;
	VulkanBuffer MaterialBuffer;

	void GenerateID();

public:

	Material();
	Material(const std::string materialName);
	Material(const std::string materialName, MaterialProperties& MaterialInfo);
	~Material();

	void Update();
	void Destroy();

	void SetBufferIndex(int bufferIndex);

	std::string GetMaterialName() { return MaterialName; }
	uint64_t GetMaterialID() { return MaterialID; }
	uint64_t GetMaterialBufferIndex() { return MaterialBufferIndex; }
	MaterialProperties* GetMaterialPropertiesPtr() { return &materialProperties; }
	VkBuffer GetMaterialBuffer() { return MaterialBuffer.GetBuffer(); }
};

