#include "RenderedTexture.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ReadableTexture.h"
#include "Texture2D.h"

RenderedTexture::RenderedTexture()
{

}

RenderedTexture::RenderedTexture(glm::ivec2 TextureResolution) : Texture(kRenderedColorTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;


	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedTexture::RenderedTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount) : Texture(kRenderedColorTexture)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;
	TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SampleCount = sampleCount;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedTexture::RenderedTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount, VkFormat format)
{
	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;
	TextureByteFormat = format;
	TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SampleCount = sampleCount;

	CreateTextureImage();
	CreateTextureView();
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedTexture::~RenderedTexture()
{
}

void RenderedTexture::CreateTextureImage()
{
	VkImageCreateInfo TextureInfo = {};
	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
	TextureInfo.extent.width = Width;
	TextureInfo.extent.height = Height;
	TextureInfo.extent.depth = 1;
	TextureInfo.mipLevels = 1;
	TextureInfo.arrayLayers = 1;
	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	TextureInfo.samples = SampleCount;
	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	TextureInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	TextureInfo.format = VK_FORMAT_R8G8B8A8_UNORM;

	Texture::CreateTextureImage(TextureInfo);
}

void RenderedTexture::CreateTextureView()
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

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void RenderedTexture::BakeDepthTexture(const char* filename, BakeTextureFormat textureFormat)
{
	std::shared_ptr<ReadableTexture> BakeTexture = std::make_shared<ReadableTexture>(ReadableTexture(glm::vec2(Width, Height), SampleCount));

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	Texture::CopyTexture(commandBuffer, this, BakeTexture.get());
	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VulkanRenderer::EndSingleTimeCommands(commandBuffer);

	VkImageSubresource subResource{ VK_IMAGE_ASPECT_DEPTH_BIT, 0, 0 };
	VkSubresourceLayout subResourceLayout;
	vkGetImageSubresourceLayout(VulkanRenderer::GetDevice(), BakeTexture->Image, &subResource, &subResourceLayout);

	const char* data;
	vkMapMemory(VulkanRenderer::GetDevice(), BakeTexture->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);

	switch (textureFormat)
	{
		case BakeTextureFormat::Bake_BMP: stbi_write_bmp(filename, Width, Height, STBI_grey, data); break;
		case BakeTextureFormat::Bake_JPG: stbi_write_jpg(filename, Width, Height, STBI_grey, data, 100); break;
		case BakeTextureFormat::Bake_PNG: stbi_write_png(filename, Width, Height, STBI_grey, data, STBI_grey * Width); break;
		case BakeTextureFormat::Bake_TGA: stbi_write_tga(filename, Width, Height, STBI_grey, data); break;
	}

	BakeTexture->Destroy();
}

std::shared_ptr<BakedTexture> RenderedTexture::BakeColorTexture(const char* filename, BakeTextureFormat textureFormat)
{
	//	std::shared_ptr<Texture2D> BakeTexture = std::make_shared<Texture2D>(Texture2D(Pixel(255, 0, 0), glm::vec2(32768.0f / 2), VkFormat::VK_FORMAT_R8G8B8A8_UNORM, TextureTypeEnum::kTextureAtlus));
	std::shared_ptr<BakedTexture> BakeTexture = std::make_shared<BakedTexture>(BakedTexture(Pixel(255, 0, 0, 255), glm::vec2(8192)));

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.layerCount = 1;

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;

	copyImage.dstOffset.x = 1024.0f;
	copyImage.dstOffset.y = 1024.0f;
	copyImage.dstOffset.z = 1;

	copyImage.extent.width = this->Width;
	copyImage.extent.height = this->Height;
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, this->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, BakeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);

	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VulkanRenderer::EndSingleTimeCommands(commandBuffer);

	VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
	VkSubresourceLayout subResourceLayout;
	vkGetImageSubresourceLayout(VulkanRenderer::GetDevice(), BakeTexture->Image, &subResource, &subResourceLayout);

	const char* data;
	vkMapMemory(VulkanRenderer::GetDevice(), BakeTexture->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);

	switch (textureFormat)
	{
	case BakeTextureFormat::Bake_BMP: stbi_write_bmp(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data); break;
	case BakeTextureFormat::Bake_JPG: stbi_write_jpg(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data, 100); break;
	case BakeTextureFormat::Bake_PNG: stbi_write_png(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data, STBI_rgb_alpha * Width); break;
	case BakeTextureFormat::Bake_TGA: stbi_write_tga(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data); break;
	}


	BakeTexture->Destroy();
	return BakeTexture;
}

void RenderedTexture::BakeEnvironmentMapTexture(const char* filename)
{
	std::shared_ptr<ReadableTexture> BakeTexture = std::make_shared<ReadableTexture>(ReadableTexture(glm::vec2(Width, Height), SampleCount, VK_FORMAT_R32G32B32A32_SFLOAT));

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	Texture::CopyTexture(commandBuffer, this, BakeTexture.get());
	BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VulkanRenderer::EndSingleTimeCommands(commandBuffer);

	VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
	VkSubresourceLayout subResourceLayout;
	vkGetImageSubresourceLayout(VulkanRenderer::GetDevice(), BakeTexture->Image, &subResource, &subResourceLayout);

	const float* data;
	vkMapMemory(VulkanRenderer::GetDevice(), BakeTexture->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);
	stbi_write_hdr(filename, Width, Height, STBI_rgb_alpha, data);
	BakeTexture->Destroy();
}
