#include "Material.h"

uint64_t Material::MaterialIDCounter = 0;

Material::Material()
{
	MaterialType = MaterialTypeEnum::kMaterialnone;
}

Material::Material(const std::string materialName)
{
	GenerateID();
	MaterialName = materialName;

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MaterialBufferUpdate();
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::Material(const std::string materialName, MaterialTypeEnum materialtype)
{
	GenerateID();
	MaterialName = materialName;
	MaterialType = materialtype;

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

		if (MaterialInfo.AlbedoMap != nullptr)
		{
			materialTextureData.AlbedoMapID = MaterialInfo.AlbedoMap->GetTextureBufferIndex();
			AlbedoMap = MaterialInfo.AlbedoMap;
		}
		if (MaterialInfo.MetallicMap != nullptr)
		{
			materialTextureData.MetallicMapID = MaterialInfo.MetallicMap->GetTextureBufferIndex();
			MetallicMap = MaterialInfo.MetallicMap;
		}
		if (MaterialInfo.RoughnessMap != nullptr)
		{
			materialTextureData.RoughnessMapID = MaterialInfo.RoughnessMap->GetTextureBufferIndex();
			RoughnessMap = MaterialInfo.RoughnessMap;
		}
		if (MaterialInfo.AmbientOcclusionMap != nullptr)
		{
			materialTextureData.AmbientOcclusionMapID = MaterialInfo.AmbientOcclusionMap->GetTextureBufferIndex();
			AmbientOcclusionMap = MaterialInfo.AmbientOcclusionMap;
		}
	}

	materialTextureData.Emission = MaterialInfo.Emission;
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

void Material::SetAlbedoMap(glm::vec4 albedo)
{
	Albedo = albedo;
	MaterialBufferUpdate();
}

void Material::SetMetallicMap(float metallic)
{
	Metallic = metallic;
	MaterialBufferUpdate();
}

void Material::SetRoughnessMap(float roughness) 
{
	Roughness = roughness;
	MaterialBufferUpdate();
}

void Material::SetAmbientOcclusionMap(float ambientOcclusion)
{
	AmbientOcclusion = ambientOcclusion;
	MaterialBufferUpdate();
}

void Material::SetEmissionMap(glm::vec3 emission)
{
	Emission = emission;
	MaterialBufferUpdate();
}

void Material::SetDiffuseMap(std::shared_ptr<Texture2D> texture)
{
	DiffuseMap = texture;
	MaterialBufferUpdate();
}

void Material::SetSpecularMap(std::shared_ptr<Texture2D> texture)
{
	SpecularMap = texture;
	MaterialBufferUpdate();
}

void Material::SetAlbedoMap(std::shared_ptr<Texture2D> texture)
{
	AlbedoMap = texture;
	MaterialBufferUpdate();
}

void Material::SetMetallicMap(std::shared_ptr<Texture2D> texture)
{
	MetallicMap = texture;
	MaterialBufferUpdate();
}

void Material::SetRoughnessMap(std::shared_ptr<Texture2D> texture)
{
	RoughnessMap = texture;
	MaterialBufferUpdate();
}

void Material::SetAmbientOcclusionMap(std::shared_ptr<Texture2D> texture)
{
	AmbientOcclusionMap = texture;
	MaterialBufferUpdate();
}

void Material::SetNormalMap(std::shared_ptr<Texture2D> texture)
{
	NormalMap = texture;
	MaterialBufferUpdate();
}

void Material::SetDepthMap(std::shared_ptr<Texture2D> texture)
{
	DepthMap = texture;
	MaterialBufferUpdate();
}

void Material::SetAlphaMap(std::shared_ptr<Texture2D> texture)
{
	AlphaMap = texture;
	MaterialBufferUpdate();
}

void Material::SetEmissionMap(std::shared_ptr<Texture2D> texture)
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