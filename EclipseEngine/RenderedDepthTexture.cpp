#include "RenderedDepthTexture.h"

RenderedDepthTexture::RenderedDepthTexture()
{

}

RenderedDepthTexture::RenderedDepthTexture(glm::ivec2 TextureResolution) : Texture(kRenderedDepthTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_D32_SFLOAT;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedDepthTexture::RenderedDepthTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount) : Texture(kRenderedDepthTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = sampleCount;
	TextureByteFormat = VK_FORMAT_D32_SFLOAT;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedDepthTexture::~RenderedDepthTexture()
{
}

void RenderedDepthTexture::CreateTextureImage()
{
	VkImageCreateInfo TextureInfo{};
	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
	TextureInfo.extent.width = Width;
	TextureInfo.extent.height = Height;
	TextureInfo.extent.depth = 1;
	TextureInfo.mipLevels = 1;
	TextureInfo.arrayLayers = 1;
	TextureInfo.format = VK_FORMAT_D32_SFLOAT;
	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	TextureInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	TextureInfo.samples = SampleCount;
	TextureInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	Texture::CreateTextureImage(TextureInfo);
}

void RenderedDepthTexture::CreateTextureView()
{
	VkImageViewCreateInfo TextureImageViewInfo{};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.image = Image;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.format = VK_FORMAT_D32_SFLOAT;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(VulkanRenderer::GetDevice(), &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void RenderedDepthTexture::CreateTextureSampler()
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

void RenderedDepthTexture::RecreateRendererTexture(glm::vec2 TextureResolution)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;

	Texture::Destroy();
	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

VkAttachmentDescription RenderedDepthTexture::GetAttachmentDescription()
{
	VkAttachmentDescription DepthAttachment = {};
	DepthAttachment.format = TextureByteFormat;
	DepthAttachment.samples = SampleCount;
	DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	return DepthAttachment;
}
