#include "CubeMapTexture.h"
#include "VulkanBuffer.h"
#include <tinygltf\stb_image.h>

CubeMapTexture::CubeMapTexture() : Texture()
{
}

CubeMapTexture::CubeMapTexture(CubeMapLayout CubeMapFiles) : Texture(TextureTypeEnum::kCubeMapTexture)
{
	FilePath = CubeMapFiles.Left;
	TextureName = CubeMapFiles.Left;
	GenerateID();

	Width = 0;
	Height = 0;
	Depth = 1;

	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	LoadTexture(CubeMapFiles, TextureByteFormat);
	CreateTextureView(TextureByteFormat);
	CreateTextureSampler();

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

CubeMapTexture::CubeMapTexture(glm::ivec2 TextureResolution, TextureTypeEnum textureType) : Texture(textureType)
{
	FilePath = "";
	TextureName = "";
	GenerateID();

	Width = TextureResolution.x;
	Height = TextureResolution.y;
	Depth = 1;

	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
	TextureByteFormat = VK_FORMAT_R8G8B8A8_UNORM;

	ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

CubeMapTexture::~CubeMapTexture()
{
}

void CubeMapTexture::LoadTexture(CubeMapLayout CubeMapFiles, VkFormat textureFormat)
{
	std::vector<byte*> textureData;
	int texChannels;

	textureData.emplace_back(stbi_load(CubeMapFiles.Left.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));
	textureData.emplace_back(stbi_load(CubeMapFiles.Right.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));
	textureData.emplace_back(stbi_load(CubeMapFiles.Top.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));
	textureData.emplace_back(stbi_load(CubeMapFiles.Bottom.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));
	textureData.emplace_back(stbi_load(CubeMapFiles.Back.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));
	textureData.emplace_back(stbi_load(CubeMapFiles.Front.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha));

	const VkDeviceSize imageSize = Width * Height * 4 * 6;
	const VkDeviceSize layerSize = imageSize \ 6;

	VulkanBuffer StagingBuffer(nullptr, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(VulkanRenderer::GetDevice(), *StagingBuffer.GetBufferMemoryPtr(), 0, imageSize, 0, &data);
	for (int x = 0; x < 6; ++x)
	{
		memcpy(static_cast<char*>(data) + (x * layerSize), textureData[x], static_cast<size_t>(layerSize));
	}
	vkUnmapMemory(VulkanRenderer::GetDevice(), *StagingBuffer.GetBufferMemoryPtr());

	VkImageCreateInfo TextureInfo = {};
	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
	TextureInfo.extent.width = Width;
	TextureInfo.extent.height = Height;
	TextureInfo.extent.depth = 1;
	TextureInfo.mipLevels = 1;
	TextureInfo.arrayLayers = 6;
	TextureInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	TextureInfo.format = textureFormat;
	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	TextureInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	TextureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	TextureInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateTextureImage(TextureInfo);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer.Buffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	StagingBuffer.DestoryBuffer();
	for (auto texturedata : textureData)
	{
		stbi_image_free(texturedata);
	}
}

void CubeMapTexture::CreateTextureView(VkFormat textureFormat)
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.image = Image;
	TextureImageViewInfo.format = textureFormat;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	TextureImageViewInfo.subresourceRange.layerCount = 6;

	if (vkCreateImageView(VulkanRenderer::GetDevice(), &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void CubeMapTexture::CreateTextureSampler()
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
	TextureImageSamplerInfo.maxAnisotropy = 16.0f;
	TextureImageSamplerInfo.minLod = 0.0f;
	TextureImageSamplerInfo.maxLod = 1.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
	TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureImageSamplerInfo.anisotropyEnable = VK_TRUE;

	if (vkCreateSampler(VulkanRenderer::GetDevice(), &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}
}