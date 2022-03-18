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

Material::Material(nlohmann::json& json)
{
	GenerateID();
	MaterialName = json.at("MaterialName").get_to(MaterialName);

	materialTextureData.Ambient.x = json.at("Ambient")[0].get_to(Ambient.x);
	materialTextureData.Ambient.y = json.at("Ambient")[1].get_to(Ambient.y);
	materialTextureData.Ambient.z = json.at("Ambient")[2].get_to(Ambient.z);
	materialTextureData.Diffuse.x = json.at("Diffuse")[0].get_to(Diffuse.x);
	materialTextureData.Diffuse.y = json.at("Diffuse")[1].get_to(Diffuse.y);
	materialTextureData.Diffuse.z = json.at("Diffuse")[2].get_to(Diffuse.z);
	materialTextureData.Specular.x = json.at("Specular")[0].get_to(Specular.x);
	materialTextureData.Specular.y = json.at("Specular")[1].get_to(Specular.y);
	materialTextureData.Specular.z = json.at("Specular")[2].get_to(Specular.z);
	materialTextureData.Shininess = json.at("Shininess").get_to(Shininess);
	materialTextureData.Reflectivness = json.at("Reflectivness").get_to(Reflectivness);

	if (json.contains("DiffuseMap"))
	{
		DiffuseMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("DiffuseMap"))));
	}
	if (json.contains("SpecularMap"))
	{
		SpecularMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("SpecularMap"))));
	}
	if (json.contains("AlbedoMap"))
	{
		AlbedoMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AlbedoMap"))));
	}
	if (json.contains("MetallicMap"))
	{
		MetallicMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("MetallicMap"))));
	}
	if (json.contains("RoughnessMap"))
	{
		RoughnessMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("RoughnessMap"))));
	}
	if (json.contains("AmbientOcclusionMap"))
	{
		AmbientOcclusionMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AmbientOcclusionMap"))));
	}
	if (json.contains("NormalMap"))
	{
		NormalMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("NormalMap"))));
	}
	if (json.contains("DepthMap"))
	{
		DepthMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("DepthMap"))));
	}
	if (json.contains("AlphaMap"))
	{
		AlphaMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("AlphaMap"))));
	}
	if (json.contains("EmissionMap"))
	{
		EmissionMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("EmissionMap"))));
	}
	if (json.contains("ShadowMap"))
	{
		ShadowMap = TextureManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(json.at("ShadowMap"))));
	}

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::~Material()
{
}

void Material::Update(float DeltaTime)
{

	if (DiffuseMap != nullptr)
	{
		materialTextureData.DiffuseMapID = DiffuseMap->GetTextureBufferIndex();
	}
	if (SpecularMap != nullptr)
	{
		materialTextureData.SpecularMapID = SpecularMap->GetTextureBufferIndex();
	}
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
	if (RoughnessMap != nullptr)
	{
		materialTextureData.RoughnessMapID = RoughnessMap->GetTextureBufferIndex();
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
	if (ShadowMap != nullptr)
	{
		materialTextureData.ShadowMapID = ShadowMap->GetTextureBufferIndex();
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
}

void Material::LoadSpecularMap(const std::string FilePath)
{
	SpecularMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kSpecularTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadAlbedoMap(const std::string FilePath)
{
	AlbedoMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB);
}

void Material::LoadMetallicMap(const std::string FilePath)
{
	MetallicMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadRoughnessMap(const std::string FilePath)
{
	RoughnessMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadAmbientOcclusionMap(const std::string FilePath)
{
	AmbientOcclusionMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadNormalMap(const std::string FilePath)
{
	NormalMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadDepthMap(const std::string FilePath)
{
	DepthMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kDepthTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadAlphaMap(const std::string FilePath)
{
	AlphaMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadEmissionMap(const std::string FilePath)
{
	EmissionMap = TextureManager::LoadTexture2D(FilePath, TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
}

void Material::LoadDiffuseMap(uint64_t TextureIndex)
{
	DiffuseMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadSpecularMap(uint64_t TextureIndex)
{
	SpecularMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadAlbedoMap(uint64_t TextureIndex)
{
	AlbedoMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadMetallicMap(uint64_t TextureIndex)
{
	MetallicMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadRoughnessMap(uint64_t TextureIndex)
{
	RoughnessMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadAmbientOcclusionMap(uint64_t TextureIndex)
{
	AmbientOcclusionMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadNormalMap(uint64_t TextureIndex)
{
	NormalMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadDepthMap(uint64_t TextureIndex)
{
	DepthMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadAlphaMap(uint64_t TextureIndex)
{
	AlphaMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadEmissionMap(uint64_t TextureIndex)
{
	EmissionMap = TextureManager::GetTexture2DByID(TextureIndex);
}

void Material::LoadDiffuseMap(std::shared_ptr<Texture2D> texture)
{
	DiffuseMap = texture;
}

void Material::LoadSpecularMap(std::shared_ptr<Texture2D> texture)
{
	SpecularMap = texture;
}

void Material::LoadAlbedoMap(std::shared_ptr<Texture2D> texture)
{
	AlbedoMap = texture;
}

void Material::LoadMetallicMap(std::shared_ptr<Texture2D> texture)
{
	MetallicMap = texture;
}

void Material::LoadRoughnessMap(std::shared_ptr<Texture2D> texture)
{
	RoughnessMap = texture;
}

void Material::LoadAmbientOcclusionMap(std::shared_ptr<Texture2D> texture)
{
	AmbientOcclusionMap = texture;
}

void Material::LoadNormalMap(std::shared_ptr<Texture2D> texture)
{
	NormalMap = texture;
}

void Material::LoadDepthMap(std::shared_ptr<Texture2D> texture)
{
	DepthMap = texture;
}

void Material::LoadAlphaMap(std::shared_ptr<Texture2D> texture)
{
	AlphaMap = texture;
}

void Material::LoadEmissionMap(std::shared_ptr<Texture2D> texture)
{
	EmissionMap = texture;
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