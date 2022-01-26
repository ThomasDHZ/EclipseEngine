#include "Texture.h"
#include "VulkanBuffer.h"

Texture::Texture()
{
}

Texture::Texture(std::string TextureLocation, VkFormat format)
{
	Width = 0;
	Height = 0;
	Depth = 1;
	TextureByteFormat = format;
	TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	LoadTexture(TextureLocation, format);
}

Texture::~Texture()
{
}

void Texture::LoadTexture(std::string TextureLocation, VkFormat format)
{
	int ColorChannels;
	stbi_uc* pixels = stbi_load(TextureLocation.c_str(), &Width, &Height, &ColorChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * 4;
	MipMapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(Width, Height)))) + 1;

	VulkanBuffer StagingBuffer;
	StagingBuffer.CreateBuffer(pixels, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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

	UpdateImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer.GetBuffer());

	StagingBuffer.DestoryBuffer();
	stbi_image_free(pixels);

	GenerateMipmaps();
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

	vkCmdCopyBufferToImage(commandBuffer, buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &BufferImage);
	VkResult result = VulkanRenderer::EndSingleTimeCommands(commandBuffer);
	if (result == VK_SUCCESS)
	{
		TextureImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	}
}

void Texture::GenerateMipmaps()
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(VulkanRenderer::GetPhysicalDevice(), TextureByteFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = Image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = Width;
	int32_t mipHeight = Height;

	for (uint32_t i = 1; i < MipMapLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = MipMapLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	VkResult result = VulkanRenderer::EndSingleTimeCommands(commandBuffer);
}

void Texture::UpdateImageLayout(VkImageLayout newImageLayout)
{
	VkImageSubresourceRange ImageSubresourceRange{};
	ImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageSubresourceRange.baseMipLevel = 0;
	ImageSubresourceRange.levelCount = 1;
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