#include "TextureAtlus.h"

TextureAtlus::TextureAtlus()
{
}

TextureAtlus::TextureAtlus(const std::string& textureLocation, const glm::ivec2& textureCellSize) : Texture2D(textureLocation, TextureTypeEnum::kTextureAtlus, VK_FORMAT_R8G8B8A8_UNORM)
{
	TextureCellSize = textureCellSize;
	TextureWidthCellCount = Width \ textureCellSize.x;
	TextureHeightCellCount = Height \ textureCellSize.y;
}

TextureAtlus::~TextureAtlus()
{
}

void TextureAtlus::CopyCellToTexture(std::shared_ptr<Texture> texture, const glm::ivec2& cellToTransfer)
{
	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	texture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset.x = (uint32_t)cellToTransfer.x * TextureCellSize.x;
	copyImage.srcOffset.y = (uint32_t)cellToTransfer.y * TextureCellSize.y;
	copyImage.srcOffset.z = 0.0f;

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset.x = 0.0f;
	copyImage.dstOffset.y = 0.0f;
	copyImage.dstOffset.z = 0.0f;

	copyImage.extent.width = texture->GetWidth();
	copyImage.extent.height = texture->GetHeight();
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, this->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, texture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	texture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VulkanRenderer::EndSingleTimeCommands(commandBuffer);
}

void TextureAtlus::CopyCellToCubeMapSide(std::shared_ptr<CubeMapTexture> texture, const glm::ivec2& cellToTransfer)
{
}

std::shared_ptr<Texture> TextureAtlus::CreateTextureFromTextureAtlus(const glm::ivec2& cellToTransfer)
{
	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(Texture2D(Pixel(0, 0, 0, 255), glm::ivec2(TextureCellSize.x, TextureCellSize.y), VkFormat::VK_FORMAT_R8G8B8A8_UNORM, TextureTypeEnum::kReadableTexture));

	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	newTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

	VkImageCopy copyImage{};
	copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.srcSubresource.layerCount = 1;
	copyImage.srcOffset.x = (uint32_t)cellToTransfer.x * TextureCellSize.x;
	copyImage.srcOffset.y = (uint32_t)cellToTransfer.y * TextureCellSize.y;
	copyImage.srcOffset.z = 0.0f;

	copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyImage.dstSubresource.layerCount = 1;
	copyImage.dstOffset.x = 0.0f;
	copyImage.dstOffset.y = 0.0f;
	copyImage.dstOffset.z = 0.0f;

	copyImage.extent.width = newTexture->GetWidth();
	copyImage.extent.height = newTexture->GetHeight();
	copyImage.extent.depth = 1;

	vkCmdCopyImage(commandBuffer, this->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, newTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
	newTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
	UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VulkanRenderer::EndSingleTimeCommands(commandBuffer);

	return newTexture;
}

std::shared_ptr<CubeMapTexture> TextureAtlus::CreateCubeMapTextureFromTextureAtlus(VkCommandBuffer& commandBuffer)
{
	return std::shared_ptr<CubeMapTexture>();
}