#include "ReadableTexture.h"

ReadableTexture::ReadableTexture()
{

}

ReadableTexture::ReadableTexture(glm::ivec2 TextureResolution) : Texture(kReadableTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

ReadableTexture::ReadableTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount) : Texture(kReadableTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = sampleCount;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

ReadableTexture::~ReadableTexture()
{
}

void ReadableTexture::CreateTextureImage()
{
    VkImageCreateInfo TextureInfo{};
    TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    TextureInfo.imageType = VK_IMAGE_TYPE_2D;
    TextureInfo.extent.width = Width;
    TextureInfo.extent.height = Height;
    TextureInfo.extent.depth = 1;
    TextureInfo.mipLevels = 1;
    TextureInfo.arrayLayers = 1;
    TextureInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    TextureInfo.tiling = VK_IMAGE_TILING_LINEAR;
    TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    TextureInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    TextureInfo.samples = SampleCount;
    vkCreateImage(VulkanRenderer::GetDevice(), &TextureInfo, nullptr, &Image);

    VkMemoryRequirements ScreenShotMemoryRequirements;
    vkGetImageMemoryRequirements(VulkanRenderer::GetDevice(), Image, &ScreenShotMemoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = ScreenShotMemoryRequirements.size;
    allocInfo.memoryTypeIndex = VulkanRenderer::GetMemoryType(ScreenShotMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    vkAllocateMemory(VulkanRenderer::GetDevice(), &allocInfo, nullptr, &Memory);
    vkBindImageMemory(VulkanRenderer::GetDevice(), Image, Memory, 0);
}

void ReadableTexture::CreateTextureView()
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.subresourceRange = {};
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;
	TextureImageViewInfo.image = Image;
	TextureImageViewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	if (vkCreateImageView(VulkanRenderer::GetDevice(), &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void ReadableTexture::CreateTextureSampler()
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

void ReadableTexture::RecreateRendererTexture(glm::vec2 TextureResolution)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;

	Texture::Destroy();
	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}