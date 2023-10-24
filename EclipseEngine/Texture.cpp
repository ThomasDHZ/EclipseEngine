#include "Texture.h"
#include "VulkanBuffer.h"
#include "ImGui/imgui_impl_vulkan.h"
#include <tinygltf/stb_image.h>



uint64_t Texture::TextureIDCounter = 0;

Texture::Texture()
{
	FilePath = "";
	TextureName = "";
	GenerateID();
	Width = 1;
	Height = 1;
	Depth = 1;

	TextureType = TextureTypeEnum::kUndefinedTexture;
	StartTextureByteFormat = VK_FORMAT_UNDEFINED;
	TextureByteFormat = VK_FORMAT_UNDEFINED;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;
}

Texture::Texture(const GLTFTextureLoader& textureLoader)
{
	GenerateID();

	if (textureLoader.TextureLoader.name.empty())
	{
		TextureName = textureLoader.TextureLoader.name;
	}
	else
	{
		TextureName = textureLoader.TextureLoader.uri;
	}
	FilePath = textureLoader.TextureLoader.uri;

	Width = textureLoader.TextureLoader.width;
	Height = textureLoader.TextureLoader.height;
	Depth = 1;

	TextureType = textureLoader.TextureType;
	StartTextureByteFormat = textureLoader.Format;
	TextureByteFormat = textureLoader.Format;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;

	LoadTexture(textureLoader);
}

Texture::Texture(const Pixel& ClearColor, const glm::ivec2& Resolution, VkFormat format, TextureTypeEnum textureType)
{
	GenerateID();

	TextureType = textureType;
	Width = Resolution.x;
	Height = Resolution.y;
	Depth = 1;

	TextureType = textureType;
	StartTextureByteFormat = format;
	TextureByteFormat = format;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;

	LoadTexture(ClearColor, Resolution, format);
}

Texture::Texture(const std::vector<Pixel> pixels, const glm::ivec2& Resolution, TextureTypeEnum textureType)
{
	TextureType = textureType;
	GenerateID();
}

Texture::Texture(TextureTypeEnum textureType)
{
	TextureType = textureType;
	GenerateID();
}

Texture::Texture(std::string TextureLocation, TextureTypeEnum textureType, VkFormat format)
{
	FilePath = TextureLocation;
	TextureName = TextureLocation;
	GenerateID();

	Width = 0;
	Height = 0;
	Depth = 1;

	TextureType = textureType;
	StartTextureByteFormat = format;
	TextureByteFormat = format;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SampleCount = VK_SAMPLE_COUNT_1_BIT;

	LoadTexture(TextureLocation, format);
}

Texture::~Texture()
{
}

void Texture::LoadTexture(const GLTFTextureLoader& textureLoader)
{
	MipMapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(textureLoader.TextureLoader.width, textureLoader.TextureLoader.height)))) + 1;

	void* image = (void*)&textureLoader.TextureLoader.image[0];
	VulkanBuffer StagingBuffer(image, textureLoader.TextureLoader.image.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkImageCreateInfo ImageCreateInfo = {};
	ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	ImageCreateInfo.extent.width = textureLoader.TextureLoader.width;
	ImageCreateInfo.extent.height = textureLoader.TextureLoader.height;
	ImageCreateInfo.extent.depth = 1;
	ImageCreateInfo.mipLevels = MipMapLevels;
	ImageCreateInfo.arrayLayers = 1;
	ImageCreateInfo.format = textureLoader.Format;
	ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateTextureImage(ImageCreateInfo);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer.GetBuffer());
	if (MipMapLevels > 1)
	{
		GenerateMipmaps();
	}
	StagingBuffer.DestroyBuffer();
}

void Texture::LoadTexture(const Pixel& ClearColor, const glm::ivec2& Resolution, VkFormat format)
{
	VkDeviceSize imageSize = Resolution.x * Resolution.y * sizeof(Pixel);
	
	std::vector<Pixel> pixels(Width * Height, ClearColor);
	VulkanBuffer StagingBuffer(&pixels[0], imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkImageCreateInfo ImageCreateInfo = {};
	ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	ImageCreateInfo.extent.width = Width;
	ImageCreateInfo.extent.height = Height;
	ImageCreateInfo.extent.depth = Depth;
	ImageCreateInfo.mipLevels = 1;
	ImageCreateInfo.arrayLayers = 1;
	ImageCreateInfo.format = format;
	ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateTextureImage(ImageCreateInfo);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer.GetBuffer());

	StagingBuffer.DestroyBuffer();
}

//void Texture::LoadTexture(const std::vector<Pixel>& pixels, const glm::ivec2& Resolution, VkFormat format)
//{
//	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
//
//	VulkanBuffer StagingBuffer;
//	StagingBuffer.CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &Pixels[0]);
//
//	MipMapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(Width, Height)))) + 1;
//
//	VkImageCreateInfo TextureInfo = {};
//	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
//	TextureInfo.extent.width = Width;
//	TextureInfo.extent.height = Height;
//	TextureInfo.extent.depth = Depth;
//	TextureInfo.mipLevels = MipMapLevels;
//	TextureInfo.arrayLayers = 1;
//	TextureInfo.format = format;
//	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	TextureInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//	TextureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//	TextureInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//
//	Texture::CreateTextureImage(TextureInfo);
//
//	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//	CopyBufferToImage(StagingBuffer.Buffer);
//
//	StagingBuffer.DestoryBuffer();
//
//	GenerateMipmaps(format);
//}

void Texture::LoadTexture(std::string TextureLocation, VkFormat format)
{
	int ColorChannels;
	stbi_uc* pixels = stbi_load(TextureLocation.c_str(), &Width, &Height, &ColorChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * 4;
	MipMapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(Width, Height)))) + 1;

	VulkanBuffer StagingBuffer(pixels, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkImageCreateInfo ImageCreateInfo = {};
	ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	ImageCreateInfo.extent.width = Width;
	ImageCreateInfo.extent.height = Height;
	ImageCreateInfo.extent.depth = Depth;
	ImageCreateInfo.mipLevels = MipMapLevels;
	ImageCreateInfo.arrayLayers = 1;
	ImageCreateInfo.format = format;
	ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateTextureImage(ImageCreateInfo);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer.GetBuffer());
	GenerateMipmaps();

	StagingBuffer.DestroyBuffer();
	stbi_image_free(pixels);
}

void Texture::CreateTextureImage(VkImageCreateInfo TextureInfo)
{
	if (vkCreateImage(VulkanRenderer::GetDevice(), &TextureInfo, nullptr, &Image)) {
		throw std::runtime_error("Failed to create Image.");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(VulkanRenderer::GetDevice(), Image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanRenderer::GetMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(VulkanRenderer::GetDevice(), &allocInfo, nullptr, &Memory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate image Memory.");
	}

	vkBindImageMemory(VulkanRenderer::GetDevice(), Image, Memory, 0);
}

void Texture::CopyBufferToImage(VkBuffer buffer)
{
	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	VkBufferImageCopy BufferImage = {};
	BufferImage.bufferOffset = 0;
	BufferImage.bufferRowLength = 0;
	BufferImage.bufferImageHeight = 0;
	BufferImage.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	BufferImage.imageSubresource.mipLevel = 0;
	BufferImage.imageSubresource.baseArrayLayer = 0;
	BufferImage.imageOffset.x = 0;
	BufferImage.imageOffset.y = 0;
	BufferImage.imageOffset.z = 0;
	BufferImage.imageExtent.width = Width;
	BufferImage.imageExtent.height = Height;
	BufferImage.imageExtent.depth = Depth;
	BufferImage.imageSubresource.layerCount = 1;
	if (TextureType == TextureTypeEnum::kCubeMapTexture)
	{
		BufferImage.imageSubresource.layerCount = 6;
	}

	vkCmdCopyBufferToImage(commandBuffer, buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &BufferImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(commandBuffer);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	}
}

void Texture::TransitionImageLayout(VkImageLayout newImageLayout)
{
	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = Image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = MipMapLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;


	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (TextureImageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (TextureImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("Unsupported layout transition.");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(commandBuffer);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::GenerateMipmaps()
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(VulkanRenderer::GetPhysicalDevice(), TextureByteFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("Texture image format does not support linear blitting.");
	}

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	VkImageMemoryBarrier ImageMemoryBarrier{};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	ImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	ImageMemoryBarrier.subresourceRange.layerCount = 1;
	ImageMemoryBarrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = Width;
	int32_t mipHeight = Height;

	for (uint32_t x = 1; x < MipMapLevels; x++) 
	{
		ImageMemoryBarrier.subresourceRange.baseMipLevel = x - 1;
		ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

		VkImageBlit ImageBlit{};
		ImageBlit.srcOffsets[0] = { 0, 0, 0 };
		ImageBlit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		ImageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageBlit.srcSubresource.mipLevel = x - 1;
		ImageBlit.srcSubresource.baseArrayLayer = 0;
		ImageBlit.srcSubresource.layerCount = 1;
		ImageBlit.dstOffsets[0] = { 0, 0, 0 };
		ImageBlit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		ImageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageBlit.dstSubresource.mipLevel = x;
		ImageBlit.dstSubresource.baseArrayLayer = 0;
		ImageBlit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &ImageBlit, VK_FILTER_LINEAR);
		TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
		TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	ImageMemoryBarrier.subresourceRange.baseMipLevel = MipMapLevels - 1;
	ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(commandBuffer);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}
}

void Texture::GenerateID()
{
	TextureIDCounter++;
	TextureID = TextureIDCounter;
}

void Texture::UpdateImageLayout(VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateImageLayout(VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateCubeMapLayout(VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateCubeMapLayout(VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateDepthImageLayout(VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateDepthImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateDepthImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateDepthCubeMapLayout(VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateDepthCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::UpdateDepthCubeMapLayout(VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier ImageMemoryBarrier = {};
	ImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	ImageMemoryBarrier.oldLayout = TextureImageLayout;
	ImageMemoryBarrier.newLayout = newImageLayout;
	ImageMemoryBarrier.image = Image;
	ImageMemoryBarrier.subresourceRange = ImageSubresourceRange;
	ImageMemoryBarrier.srcAccessMask = 0;
	ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdPipelineBarrier(SingleCommand, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = newImageLayout;
	}
}

void Texture::UpdateDepthCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ImageSubresourceRange.baseMipLevel = MipLevel;
	ImageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	ImageSubresourceRange.layerCount = 6;

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TextureImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.image = Image;
	barrier.subresourceRange = ImageSubresourceRange;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	TextureImageLayout = newImageLayout;
}

void Texture::Destroy()
{
	vkDestroyImageView(VulkanRenderer::GetDevice(), View, nullptr);
	vkDestroyImage(VulkanRenderer::GetDevice(), Image, nullptr);
	vkFreeMemory(VulkanRenderer::GetDevice(), Memory, nullptr);
	vkDestroySampler(VulkanRenderer::GetDevice(), Sampler, nullptr);

	View = VK_NULL_HANDLE;
	Image = VK_NULL_HANDLE;
	Memory = VK_NULL_HANDLE;
	Sampler = VK_NULL_HANDLE;
}

void Texture::UpdateTextureBufferIndex(uint64_t bufferIndex)
{
	TextureBufferIndex = bufferIndex;
}

void Texture::CopyTexture(VkCommandBuffer& commandBuffer, Texture* srcTexture, Texture* dstTexture)
{
	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height = srcTexture->Height;
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyTexture(VkCommandBuffer& commandBuffer, Texture* srcTexture, Texture* dstTexture, uint32_t MipLevel)
{
	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstSubresource.mipLevel = MipLevel;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = (uint32_t)static_cast<float>(srcTexture->Width * std::pow(0.5f, MipLevel));
	copyImage.extent.height = (uint32_t)static_cast<float>(srcTexture->Height * std::pow(0.5f, MipLevel));
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	CopyTexture(commandBuffer, srcTexture.get(), dstTexture.get());
}

void Texture::CopyTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel)
{
	CopyTexture(commandBuffer, srcTexture.get(), dstTexture.get(), MipLevel);
}

void Texture::CopyMipLevelToTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;

	copyImage.extent.width = dstTexture->Width;
	copyImage.extent.height = dstTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyMipLevelToCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 6;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 6;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = dstTexture->Width;
	copyImage.extent.height = dstTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyDepthTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height = srcTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyCubeMap(std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 6;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 6;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height = srcTexture->Height;
	copyImage.extent.depth = 1;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdCopyImage(SingleCommand, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
}

void Texture::CopyCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 6;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 6;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height = srcTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 6;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = MipLevel;
	copyImage.dstSubresource.layerCount = 6;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = (uint32_t)static_cast<float>(srcTexture->Width * std::pow(0.5f, MipLevel));
	copyImage.extent.height = (uint32_t)static_cast<float>(srcTexture->Height * std::pow(0.5f, MipLevel));
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyCubeSideToTextureMap(std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdCopyImage(SingleCommand, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
}

void Texture::CopyCubeSideToTextureMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyDepthCubeSideToTextureMap(std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdCopyImage(SingleCommand, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
}

void Texture::CopyDepthCubeSideToTextureMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyDepthCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 6;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 6;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height = srcTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyDepthCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel)
{
	VkImageCopy copyRegion = {};
	copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyRegion.srcSubresource.baseArrayLayer = 0;
	copyRegion.srcSubresource.mipLevel = 0;
	copyRegion.srcSubresource.layerCount = 6;
	copyRegion.srcOffset = { 0, 0, 0 };

	copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyRegion.dstSubresource.baseArrayLayer = 0;
	copyRegion.dstSubresource.mipLevel = MipLevel;
	copyRegion.dstSubresource.layerCount = 6;
	copyRegion.dstOffset = { 0, 0, 0 };

	copyRegion.extent.width = (uint32_t)static_cast<float>(srcTexture->Width * std::pow(0.5f, MipLevel));
	copyRegion.extent.height = (uint32_t)static_cast<float>(srcTexture->Height * std::pow(0.5f, MipLevel));
	copyRegion.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}

void Texture::CopyCubeSideToDepthMap(std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	auto SingleCommand = VulkanRenderer::BeginSingleTimeCommands();
	vkCmdCopyImage(SingleCommand, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(SingleCommand);
}

void Texture::CopyCubeSideToDepthMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcCubeTexture, int side, std::shared_ptr<Texture> dstTexture)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.srcSubresource.baseArrayLayer = side;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	copyImage.dstSubresource.baseArrayLayer = 0;
	copyImage.dstSubresource.mipLevel = 0;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcCubeTexture->Width;
	copyImage.extent.height = srcCubeTexture->Height;
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, srcCubeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::CopyCubeMapLayer(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t layer, uint32_t MipLevel)
{
	VkImageCopy copyImage = {};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.baseArrayLayer = 0;
	copyImage.srcSubresource.mipLevel = 0;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset = { 0, 0, 0 };

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.baseArrayLayer = layer;
	copyImage.dstSubresource.mipLevel = MipLevel;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset = { 0, 0, 0 };

	copyImage.extent.width = srcTexture->Width;
	copyImage.extent.height =srcTexture->Height;
	copyImage.extent.depth = 1;
	vkCmdCopyImage(commandBuffer, srcTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
}

void Texture::ImGuiShowTexture(const ImVec2& TextureDisplaySize)
{
	ImGui::Image(ImGuiDescriptorSet, TextureDisplaySize);
}
