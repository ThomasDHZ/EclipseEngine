#include "Texture2D.h"

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(nlohmann::json& json) : Texture(json)
{
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	CreateTextureView(StartTextureByteFormat);
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

}


Texture2D::Texture2D(const std::string TextureLocation, TextureTypeEnum textureType, VkFormat format) : Texture(TextureLocation, textureType, format)
{
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	CreateTextureView(format);
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

Texture2D::~Texture2D()
{
}

void Texture2D::CreateTextureView(VkFormat format)
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.format = format;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = MipMapLevels;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;
	TextureImageViewInfo.image = Image;

	if (vkCreateImageView(VulkanRenderer::GetDevice(), &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}


void Texture2D::CreateTextureSampler()
{
	VkSamplerCreateInfo TextureImageSamplerInfo = {};
	TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureImageSamplerInfo.magFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.minFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.anisotropyEnable = VK_TRUE;
	TextureImageSamplerInfo.maxAnisotropy = 16.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	TextureImageSamplerInfo.unnormalizedCoordinates = VK_FALSE;
	TextureImageSamplerInfo.compareEnable = VK_FALSE;
	TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	TextureImageSamplerInfo.minLod = 0;
	TextureImageSamplerInfo.maxLod = static_cast<float>(MipMapLevels);
	TextureImageSamplerInfo.mipLodBias = 0;

	if (vkCreateSampler(VulkanRenderer::GetDevice(), &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}
}
