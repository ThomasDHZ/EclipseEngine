#include "RenderedTexture.h"

RenderedTexture::RenderedTexture()
{

}

RenderedTexture::RenderedTexture(glm::ivec2 TextureResolution, RenderedTextureType type) : Texture()
{

	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;
}

RenderedTexture::RenderedTexture(glm::ivec2 TextureResolution, RenderedTextureType type, VkSampleCountFlagBits sampleCount) : Texture()
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkSampleCountFlagBits SampleCount = sampleCount;
}

RenderedTexture::~RenderedTexture()
{
}

void RenderedTexture::CreateTextureImage()
{
	VkImageCreateInfo TextureInfo = {};
	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
	TextureInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	TextureInfo.extent.width = Width;
	TextureInfo.extent.height = Height;
	TextureInfo.extent.depth = 1;
	TextureInfo.mipLevels = 1;
	TextureInfo.arrayLayers = 1;
	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	TextureInfo.samples = SampleCount;
	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	TextureInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	Texture::CreateTextureImage(TextureInfo);
}

void RenderedTexture::CreateTextureView()
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageViewInfo.subresourceRange = {};
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;
	TextureImageViewInfo.image = Image;

	if (vkCreateImageView(VulkanRenderer::GetDevice(), &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void RenderedTexture::CreateTextureSampler()
{
	VkSamplerCreateInfo TextureImageSamplerInfo = {};
	TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureImageSamplerInfo.magFilter = VK_FILTER_LINEAR;
	TextureImageSamplerInfo.minFilter = VK_FILTER_LINEAR;
	TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.mipLodBias = 0.0f;
	TextureImageSamplerInfo.maxAnisotropy = 1.0f;
	TextureImageSamplerInfo.minLod = 0.0f;
	TextureImageSamplerInfo.maxLod = 1.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	if (vkCreateSampler(VulkanRenderer::GetDevice(), &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}
}

void RenderedTexture::RecreateRendererTexture(glm::vec2 TextureResolution)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;

	Texture::Destroy();
	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();
}
