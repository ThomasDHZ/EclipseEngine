#include "Material.h"

uint64_t Material::MaterialIDCounter = 0;

Material::Material()
{
}

Material::Material(const std::string& materialName)
{
	MaterialName = materialName;
	MaterialIDCounter++;
	MaterialBufferIndex = MaterialIDCounter;
	MaterialBuffer = VulkanBuffer(&MaterialBuffer, sizeof(GLTFMaterialBufferInfo), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Material::~Material()
{
}

void Material::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}

void Material::UpdateMaterialBufferIndex(uint64_t bufferIndex)
{
	MaterialBufferIndex = bufferIndex;
}

void Material::UpdateBuffer()
{
	MaterialInfo.Albedo = Albedo;
	MaterialInfo.Metallic = Metallic;
	MaterialInfo.Roughness = Roughness;
	MaterialInfo.AmbientOcclusion = AmbientOcclusion;
	MaterialInfo.Emission = Emission;
	MaterialInfo.Alpha = Alpha;
	MaterialInfo.CheckPaletteSwapColor = CheckPaletteSwapColor;

	if (AlbedoMap != nullptr)
	{
		MaterialInfo.AlbedoMap = AlbedoMap->GetTextureBufferIndex();
	}
	if (MetallicRoughnessMap != nullptr)
	{
		MaterialInfo.MetallicRoughnessMap = MetallicRoughnessMap->GetTextureBufferIndex();
	}
	if (MetallicMap != nullptr)
	{
		MaterialInfo.MetallicMap = MetallicMap->GetTextureBufferIndex();
	}
	if (RoughnessMap != nullptr)
	{
		MaterialInfo.RoughnessMap = RoughnessMap->GetTextureBufferIndex();
	}
	if (AmbientOcclusionMap != nullptr)
	{
		MaterialInfo.AmbientOcclusionMap = AmbientOcclusionMap->GetTextureBufferIndex();
	}
	if (NormalMap != nullptr)
	{
		MaterialInfo.NormalMap = NormalMap->GetTextureBufferIndex();
	}
	if (DepthMap != nullptr)
	{
		MaterialInfo.DepthMap = DepthMap->GetTextureBufferIndex();
	}
	if (AlphaMap != nullptr)
	{
		MaterialInfo.AlphaMap = AlphaMap->GetTextureBufferIndex();
	}
	if (EmissionMap != nullptr)
	{
		MaterialInfo.EmissionMap = EmissionMap->GetTextureBufferIndex();
	}
	if (HeightMap != nullptr)
	{
		MaterialInfo.HeightMap = HeightMap->GetTextureBufferIndex();
	}
	if (CheckPaletteSwapColorMap != nullptr)
	{
		MaterialInfo.CheckPaletteSwapColorMap = CheckPaletteSwapColorMap->GetTextureBufferIndex();
	}
	if (PaletteSwapPixelColorMap != nullptr)
	{
		MaterialInfo.PaletteSwapPixelColorMap = PaletteSwapPixelColorMap->GetTextureBufferIndex();
	}

	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::Destroy()
{
	MaterialBuffer.DestroyBuffer();
}

void Material::GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MaterialBufferList)
{
	VkDescriptorBufferInfo MaterialBufferInfo = {};
	MaterialBufferInfo.buffer = MaterialBuffer.Buffer;
	MaterialBufferInfo.offset = 0;
	MaterialBufferInfo.range = VK_WHOLE_SIZE;
	MaterialBufferList.emplace_back(MaterialBufferInfo);
}
