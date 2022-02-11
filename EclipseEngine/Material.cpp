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

void Material::Update(float DeltaTime)
{
	materialTextureData.DiffuseMapID = DiffuseMapID;
	materialTextureData.SpecularMapID = SpecularMapID;
	materialTextureData.NormalMapID = NormalMapID;
	materialTextureData.DepthMapID = DepthMapID;
	materialTextureData.AlphaMapID = AlphaMapID;
	materialTextureData.EmissionMapID = EmissionMapID;
	materialTextureData.ShadowMapID = ShadowMapID;
	MaterialBuffer.CopyBufferToMemory(&materialTextureData, sizeof(MaterialBufferData));
}

void Material::Destroy()
{
	MaterialBuffer.DestoryBuffer();
}

void Material::LoadDiffuseMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_SRGB);
	DiffuseMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadSpecularMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
	SpecularMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadNormalMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
	NormalMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadDepthMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
	DepthMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadAlphaMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
	AlphaMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadEmissionMap(const std::string FilePath)
{
	auto texture = TextureManager::LoadTexture2D(FilePath, VK_FORMAT_R8G8B8A8_UNORM);
	EmissionMapID = TextureManager::GetTexture2DByID(texture)->GetTextureBufferIndex();
}

void Material::LoadDiffuseMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		DiffuseMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		DiffuseMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadSpecularMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		SpecularMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		SpecularMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadNormalMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		NormalMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		NormalMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadDepthMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		DepthMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		DepthMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadAlphaMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		AlphaMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		AlphaMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadEmissionMap(uint64_t TextureIndex)
{
	const std::shared_ptr<Texture2D> texture = TextureManager::GetTexture2DByID(TextureIndex);
	if (texture == nullptr)
	{
		EmissionMapID = texture->GetTextureBufferIndex();
	}
	else
	{
		EmissionMapID = TextureManager::GetTexture2DByID(TextureIndex)->GetTextureBufferIndex();
	}
}

void Material::LoadDiffuseMap(std::shared_ptr<Texture2D> texture)
{
	DiffuseMapID = texture->GetTextureBufferIndex();
}

void Material::LoadSpecularMap(std::shared_ptr<Texture2D> texture)
{
	SpecularMapID = texture->GetTextureBufferIndex();
}

void Material::LoadNormalMap(std::shared_ptr<Texture2D> texture)
{
	NormalMapID = texture->GetTextureBufferIndex();
}

void Material::LoadDepthMap(std::shared_ptr<Texture2D> texture)
{
	DepthMapID = texture->GetTextureBufferIndex();
}

void Material::LoadAlphaMap(std::shared_ptr<Texture2D> texture)
{
	AlphaMapID = texture->GetTextureBufferIndex();
}

void Material::LoadEmissionMap(std::shared_ptr<Texture2D> texture)
{
	EmissionMapID = texture->GetTextureBufferIndex();
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