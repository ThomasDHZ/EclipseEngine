#include "GLTFMaterial.h"

uint64_t GLTFMaterial::MaterialIDCounter = 0;

GLTFMaterial::GLTFMaterial()
{
}

GLTFMaterial::GLTFMaterial(const std::string& materialName)
{
	MaterialName = materialName;
	MaterialIDCounter++;
	MaterialBufferIndex = MaterialIDCounter;
	MaterialBuffer = VulkanBuffer(&MaterialBuffer, sizeof(GLTFMaterialBufferInfo), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

GLTFMaterial::~GLTFMaterial()
{
}

void GLTFMaterial::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}

void GLTFMaterial::UpdateMaterialBufferIndex(uint64_t bufferIndex)
{
	MaterialBufferIndex = bufferIndex;
}

void GLTFMaterial::UpdateBuffer()
{
	MaterialInfo.Albedo = Albedo;
	MaterialInfo.Metallic = Metallic;
	MaterialInfo.Roughness = Roughness;
	MaterialInfo.AmbientOcclusion = AmbientOcclusion;
	MaterialInfo.Emission = Emission;
	MaterialInfo.Alpha = Alpha;

	if (AlbedoMap != nullptr)
	{
		MaterialInfo.AlbedoMap = AlbedoMap->GetTextureBufferIndex();
	}
	if (MetallicRoughnessMap != nullptr)
	{
		MaterialInfo.MetallicRoughnessMap = MetallicRoughnessMap->GetTextureBufferIndex();
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

	MaterialBuffer.CopyBufferToMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void GLTFMaterial::GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MaterialBufferList)
{
	VkDescriptorBufferInfo MaterialBufferInfo = {};
	MaterialBufferInfo.buffer = MaterialBuffer.Buffer;
	MaterialBufferInfo.offset = 0;
	MaterialBufferInfo.range = VK_WHOLE_SIZE;
	MaterialBufferList.emplace_back(MaterialBufferInfo);
}
