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
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::Material(const std::string materialName, MaterialProperties& MaterialInfo)
{
	GenerateID();
	MaterialName = materialName;
	
	materialTextureData.Ambient = MaterialInfo.Ambient;
	materialTextureData.Diffuse = MaterialInfo.Diffuse;
	materialTextureData.Specular = MaterialInfo.Specular;
	materialTextureData.Shininess = MaterialInfo.Shininess;
	materialTextureData.Reflectivness = MaterialInfo.Reflectivness;

	if(MaterialInfo.DiffuseMap != nullptr)
	{
		materialTextureData.DiffuseMapID = MaterialInfo.DiffuseMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.SpecularMap != nullptr)
	{
		materialTextureData.SpecularMapID = MaterialInfo.SpecularMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.NormalMap != nullptr)
	{
		materialTextureData.NormalMapID = MaterialInfo.NormalMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.DepthMap != nullptr)
	{
		materialTextureData.DepthMapID = MaterialInfo.DepthMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.AlphaMap != nullptr)
	{
		materialTextureData.AlphaMapID = MaterialInfo.AlphaMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.EmissionMap != nullptr)
	{
		materialTextureData.EmissionMapID = MaterialInfo.EmissionMap->GetTextureBufferIndex();
	}
	if (MaterialInfo.ShadowMap != nullptr)
	{
		materialTextureData.ShadowMapID = MaterialInfo.ShadowMap->GetTextureBufferIndex();
	}

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::~Material()
{
}

void Material::Update()
{
	MaterialBuffer.CopyBufferToMemory(&materialTextureData, sizeof(MaterialBufferData));
}

void Material::Destroy()
{
	MaterialBuffer.DestoryBuffer();
}

void Material::LoadDiffuseMap(const std::string FilePath)
{
	materialTextureData.DiffuseMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_SRGB);
}

void Material::LoadSpecularMap(const std::string FilePath)
{
	materialTextureData.SpecularMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadNormalMap(const std::string FilePath)
{
	materialTextureData.NormalMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadDepthMap(const std::string FilePath)
{
	materialTextureData.DepthMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadAlphaMap(const std::string FilePath)
{
	materialTextureData.AlphaMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadEmissionMap(const std::string FilePath)
{
	materialTextureData.EmissionMapID = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadDiffuseMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.DiffuseMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.DiffuseMapID = materialTextureData.DiffuseMapID;
	}
}

void Material::LoadSpecularMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.SpecularMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.SpecularMapID = materialTextureData.DiffuseMapID;
	}
}

void Material::LoadNormalMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.NormalMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.NormalMapID = materialTextureData.DiffuseMapID;
	}
}

void Material::LoadDepthMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.DepthMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.DepthMapID = DefaultTextureID;
	}
}

void Material::LoadAlphaMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.AlphaMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.AlphaMapID = DefaultAlphaTextureID;
	}
}

void Material::LoadEmissionMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		materialTextureData.EmissionMapID = texture->GetTextureID();
	}
	else
	{
		materialTextureData.EmissionMapID = DefaultTextureID;
	}
}

void Material::LoadDiffuseMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
}

void Material::LoadSpecularMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
}

void Material::LoadNormalMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
}

void Material::LoadDepthMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
}

void Material::LoadAlphaMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
}

void Material::LoadEmissionMap(std::shared_ptr<Texture2D> texture)
{
	materialTextureData.EmissionMapID = texture->GetTextureID();
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