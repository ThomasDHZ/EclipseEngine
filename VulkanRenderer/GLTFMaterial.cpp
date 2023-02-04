#include "GLTFMaterial.h"

uint64_t GLTFMaterial::MaterialIDCounter = 0;

GLTFMaterial::GLTFMaterial()
{
}

GLTFMaterial::GLTFMaterial(const std::string& materialName)
{
	MaterialName = materialName;
}

GLTFMaterial::~GLTFMaterial()
{
}

VkDescriptorImageInfo GLTFMaterial::GetAlbedoMapDescriptor()
{
	if (AlbedoMap != nullptr)
	{
		VkDescriptorImageInfo albedoTextureDescriptor{};
		albedoTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		albedoTextureDescriptor.imageView = AlbedoMap->GetView();
		albedoTextureDescriptor.sampler = AlbedoMap->GetSampler();
		return albedoTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetMetallicRoughnessMapDescriptor()
{
	if (MetallicRoughnessMap != nullptr)
	{
		VkDescriptorImageInfo metallicRoughnessTextureDescriptor{};
		metallicRoughnessTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		metallicRoughnessTextureDescriptor.imageView = MetallicRoughnessMap->GetView();
		metallicRoughnessTextureDescriptor.sampler = MetallicRoughnessMap->GetSampler();
		return metallicRoughnessTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetAmbientOcclusionMapDescriptor()
{
	if (AmbientOcclusionMap != nullptr)
	{
		VkDescriptorImageInfo ambientOcclusionMapTextureDescriptor{};
		ambientOcclusionMapTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		ambientOcclusionMapTextureDescriptor.imageView = AmbientOcclusionMap->GetView();
		ambientOcclusionMapTextureDescriptor.sampler = AmbientOcclusionMap->GetSampler();
		return ambientOcclusionMapTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetNormalMapDescriptor()
{
	if (NormalMap != nullptr)
	{
		VkDescriptorImageInfo normalTextureDescriptor{};
		normalTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		normalTextureDescriptor.imageView = NormalMap->GetView();
		normalTextureDescriptor.sampler = NormalMap->GetSampler();
		return normalTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetDepthMapDescriptor()
{
	if (DepthMap != nullptr)
	{
		VkDescriptorImageInfo depthTextureDescriptor{};
		depthTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		depthTextureDescriptor.imageView = DepthMap->GetView();
		depthTextureDescriptor.sampler = DepthMap->GetSampler();
		return depthTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetAlphaMapDescriptor()
{
	if (AlphaMap != nullptr)
	{
		VkDescriptorImageInfo alphaTextureDescriptor{};
		alphaTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		alphaTextureDescriptor.imageView = AlphaMap->GetView();
		alphaTextureDescriptor.sampler = AlphaMap->GetSampler();
		return alphaTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFMaterial::GetEmissionMapDescriptor()
{
	if (AlphaMap != nullptr)
	{
		VkDescriptorImageInfo alphaTextureDescriptor{};
		alphaTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		alphaTextureDescriptor.imageView = AlphaMap->GetView();
		alphaTextureDescriptor.sampler = AlphaMap->GetSampler();
		return alphaTextureDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

void GLTFMaterial::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}
