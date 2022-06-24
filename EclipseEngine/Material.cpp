#include "Material.h"

uint64_t Material::MaterialIDCounter = 0;

Material::Material()
{
}

Material::Material(const std::string materialName)
{
	GenerateID();
	MaterialName = materialName;

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MaterialBufferUpdate();
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::Material(const std::string materialName, MaterialTypeEnum materialtype, MaterialProperties& MaterialInfo)
{
	GenerateID();
	MaterialName = materialName;
	MaterialType = materialtype;
	
	if (MaterialType == MaterialTypeEnum::kMaterialBlinnPhong)
	{
		materialTextureData.Ambient = MaterialInfo.Ambient;
		materialTextureData.Diffuse = MaterialInfo.Diffuse;
		materialTextureData.Specular = MaterialInfo.Specular;
		materialTextureData.Shininess = MaterialInfo.Shininess;
		materialTextureData.Reflectivness = MaterialInfo.Reflectivness;

		if (MaterialInfo.DiffuseMap != nullptr)
		{
			materialTextureData.DiffuseMapID = MaterialInfo.DiffuseMap->GetTextureBufferIndex();
			DiffuseMap = MaterialInfo.DiffuseMap;
		}
		if (MaterialInfo.SpecularMap != nullptr)
		{
			materialTextureData.SpecularMapID = MaterialInfo.SpecularMap->GetTextureBufferIndex();
			SpecularMap = MaterialInfo.SpecularMap;
		}
	}
	else
	{
		materialTextureData.Albedo = MaterialInfo.Albedo;
		materialTextureData.Metallic = MaterialInfo.Metallic;
		materialTextureData.Roughness = MaterialInfo.Roughness;
		materialTextureData.AmbientOcclusion = MaterialInfo.AmbientOcclusion;
		materialTextureData.Reflectivness = MaterialInfo.Reflectivness;

		if (AlbedoMap != nullptr)
		{
			materialTextureData.AlbedoMapID = AlbedoMap->GetTextureBufferIndex();
			AlbedoMap = MaterialInfo.AlbedoMap;
		}
		if (MetallicMap != nullptr)
		{
			materialTextureData.MetallicMapID = MetallicMap->GetTextureBufferIndex();
			MetallicMap = MaterialInfo.MetallicMap;
		}
		if (RoughnessMap != nullptr)
		{
			materialTextureData.RoughnessMapID = RoughnessMap->GetTextureBufferIndex();
			RoughnessMap = MaterialInfo.RoughnessMap;
		}
		if (AmbientOcclusionMap != nullptr)
		{
			materialTextureData.AmbientOcclusionMapID = AmbientOcclusionMap->GetTextureBufferIndex();
			AmbientOcclusionMap = MaterialInfo.AmbientOcclusionMap;
		}
	}

	materialTextureData.Alpha = MaterialInfo.Alpha;
	if (MaterialInfo.NormalMap != nullptr)
	{
		materialTextureData.NormalMapID = MaterialInfo.NormalMap->GetTextureBufferIndex();
		NormalMap = MaterialInfo.NormalMap;
	}
	if (MaterialInfo.DepthMap != nullptr)
	{
		materialTextureData.DepthMapID = MaterialInfo.DepthMap->GetTextureBufferIndex();
		DepthMap = MaterialInfo.DepthMap;
	}
	if (MaterialInfo.AlphaMap != nullptr)
	{
		materialTextureData.AlphaMapID = MaterialInfo.AlphaMap->GetTextureBufferIndex();
		AlphaMap = MaterialInfo.AlphaMap;
	}
	if (MaterialInfo.EmissionMap != nullptr)
	{
		materialTextureData.EmissionMapID = MaterialInfo.EmissionMap->GetTextureBufferIndex();
		EmissionMap = MaterialInfo.EmissionMap;
	}

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MaterialBufferUpdate();
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::~Material()
{
}

void Material::MaterialBufferUpdate()
{
	if (MaterialType == MaterialTypeEnum::kMaterialBlinnPhong)
	{
		if (DiffuseMap != nullptr)
		{
			materialTextureData.DiffuseMapID = DiffuseMap->GetTextureBufferIndex();
		}
		if (SpecularMap != nullptr)
		{
			materialTextureData.SpecularMapID = SpecularMap->GetTextureBufferIndex();
		}
	}
	else
	{
		if (AlbedoMap != nullptr)
		{
			materialTextureData.AlbedoMapID = AlbedoMap->GetTextureBufferIndex();
		}
		if (MetallicMap != nullptr)
		{
			materialTextureData.MetallicMapID = MetallicMap->GetTextureBufferIndex();
		}
		if (RoughnessMap != nullptr)
		{
			materialTextureData.RoughnessMapID = RoughnessMap->GetTextureBufferIndex();
		}
		if (AmbientOcclusionMap != nullptr)
		{
			materialTextureData.AmbientOcclusionMapID = AmbientOcclusionMap->GetTextureBufferIndex();
		}
	}


	if (NormalMap != nullptr)
	{
		materialTextureData.NormalMapID = NormalMap->GetTextureBufferIndex();
	}
	if (DepthMap != nullptr)
	{
		materialTextureData.DepthMapID = DepthMap->GetTextureBufferIndex();
	}
	if (AlphaMap != nullptr)
	{
		materialTextureData.AlphaMapID = AlphaMap->GetTextureBufferIndex();
	}
	if (EmissionMap != nullptr)
	{
		materialTextureData.EmissionMapID = EmissionMap->GetTextureBufferIndex();
	}

	MaterialBuffer.CopyBufferToMemory(&materialTextureData, sizeof(MaterialBufferData));
}

void Material::Destroy()
{
	MaterialBuffer.DestoryBuffer();
}

void Material::LoadDiffuseMap(const std::string FilePath)
{
	DiffuseMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kDiffuseTextureMap, VK_FORMAT_R8G8B8A8_SRGB);
	MaterialBufferUpdate();
}

void Material::LoadSpecularMap(const std::string FilePath)
{
	SpecularMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kSpecularTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadAlbedoMap(const std::string FilePath)
{
	AlbedoMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadMetallicMap(const std::string FilePath)
{
	MetallicMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadRoughnessMap(const std::string FilePath)
{
	RoughnessMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadAmbientOcclusionMap(const std::string FilePath)
{
	AmbientOcclusionMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadNormalMap(const std::string FilePath)
{
	NormalMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadDepthMap(const std::string FilePath)
{
	DepthMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kDepthTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadAlphaMap(const std::string FilePath)
{
	AlphaMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadEmissionMap(const std::string FilePath)
{
	EmissionMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	MaterialBufferUpdate();
}

void Material::LoadDiffuseMap(uint64_t TextureIndex)
{
	DiffuseMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadSpecularMap(uint64_t TextureIndex)
{
	SpecularMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadAlbedoMap(uint64_t TextureIndex)
{
	AlbedoMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadMetallicMap(uint64_t TextureIndex)
{
	MetallicMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadRoughnessMap(uint64_t TextureIndex)
{
	RoughnessMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadAmbientOcclusionMap(uint64_t TextureIndex)
{
	AmbientOcclusionMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadNormalMap(uint64_t TextureIndex)
{
	NormalMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadDepthMap(uint64_t TextureIndex)
{
	DepthMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadAlphaMap(uint64_t TextureIndex)
{
	AlphaMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadEmissionMap(uint64_t TextureIndex)
{
	EmissionMap = TextureManager::GetTexture2DByID(TextureIndex);
	MaterialBufferUpdate();
}

void Material::LoadDiffuseMap(std::shared_ptr<Texture2D> texture)
{
	DiffuseMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadSpecularMap(std::shared_ptr<Texture2D> texture)
{
	SpecularMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadAlbedoMap(std::shared_ptr<Texture2D> texture)
{
	AlbedoMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadMetallicMap(std::shared_ptr<Texture2D> texture)
{
	MetallicMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadRoughnessMap(std::shared_ptr<Texture2D> texture)
{
	RoughnessMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadAmbientOcclusionMap(std::shared_ptr<Texture2D> texture)
{
	AmbientOcclusionMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadNormalMap(std::shared_ptr<Texture2D> texture)
{
	NormalMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadDepthMap(std::shared_ptr<Texture2D> texture)
{
	DepthMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadAlphaMap(std::shared_ptr<Texture2D> texture)
{
	AlphaMap = texture;
	MaterialBufferUpdate();
}

void Material::LoadEmissionMap(std::shared_ptr<Texture2D> texture)
{
	EmissionMap = texture;
	MaterialBufferUpdate();
}

void Material::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}

void Material::SetBufferIndex(int bufferIndex)
{
	MaterialBufferIndex = bufferIndex;
}