#pragma once
#include <vector>
#include <memory>
#include <Texture2D.h>
#include <CubeMapTexture.h>
#include <EnvironmentTexture.h>
#include <TextureAtlus.h>

class TextureManager
{
private:
	static VkSampler NullSampler;
	static std::vector<std::shared_ptr<Texture2D>> Texture2DList;

	static std::shared_ptr<Texture2D> IsTexture2DLoaded(std::string name)
	{
		uint64_t textureID = 0;
		for (auto texture : Texture2DList)
		{
			if (texture->GetFilePath() == name)
			{
				return texture;
			}
		}

		return nullptr;
	}

	static void UpdateBufferIndex()
	{
		for (int x = 0; x < Texture2DList.size(); x++)
		{
			Texture2DList[x]->SetTextureBufferIndex(x);
		}
	}

public: 
	static std::shared_ptr<EnvironmentTexture> EnvironmentTexture;

	static void StartUp()
	{
		LoadTexture2D("../texture/DefaultTexture.png", TextureTypeEnum::kDiffuseTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
		LoadTexture2D("../texture/AlphaDefault.png", TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM);

		VkSamplerCreateInfo NullSamplerInfo = {};
		NullSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		NullSamplerInfo.magFilter = VK_FILTER_NEAREST;
		NullSamplerInfo.minFilter = VK_FILTER_NEAREST;
		NullSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.anisotropyEnable = VK_TRUE;
		NullSamplerInfo.maxAnisotropy = 16.0f;
		NullSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		NullSamplerInfo.unnormalizedCoordinates = VK_FALSE;
		NullSamplerInfo.compareEnable = VK_FALSE;
		NullSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		NullSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		NullSamplerInfo.minLod = 0;
		NullSamplerInfo.maxLod = 0;
		NullSamplerInfo.mipLodBias = 0;

		if (vkCreateSampler(VulkanRenderer::GetDevice(), &NullSamplerInfo, nullptr, &NullSampler))
		{
			throw std::runtime_error("Failed to create Sampler.");
		}
	}

	static std::shared_ptr<Texture2D> LoadTexture2D(std::shared_ptr<Texture2D> texture)
	{
		std::shared_ptr<Texture2D> isTextureLoaded = IsTexture2DLoaded(texture->GetFilePath());
		if (isTextureLoaded != nullptr)
		{
			return isTextureLoaded;
		}

		Texture2DList.emplace_back(texture);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return texture;
	}

	static std::shared_ptr<Texture2D> LoadTexture2D(const std::string TextureLocation, TextureTypeEnum textureType, VkFormat format)
	{
		std::shared_ptr<Texture2D> isTextureLoaded = IsTexture2DLoaded(TextureLocation);
		if (isTextureLoaded != nullptr)
		{
			return isTextureLoaded;
		}

		const std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(Texture2D(TextureLocation, textureType, format));
		Texture2DList.emplace_back(texture);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return texture;
	}

	static std::shared_ptr<TextureAtlus> LoadTextureAtlus(const std::string& TextureLocation, const glm::ivec2& TextureCellSize)
	{
		const std::shared_ptr<TextureAtlus> texture = std::make_shared<TextureAtlus>(TextureAtlus(TextureLocation, TextureCellSize));
		Texture2DList.emplace_back(texture);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return texture;
	}

	static std::vector<VkDescriptorImageInfo>  GetTexturemBufferList()
	{
		std::vector<VkDescriptorImageInfo> DescriptorImageList;
		if (Texture2DList.size() == 0)
		{
			VkDescriptorImageInfo nullBuffer;
			nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			nullBuffer.imageView = VK_NULL_HANDLE;
			nullBuffer.sampler = NullSampler;
			DescriptorImageList.emplace_back(nullBuffer);
		}
		else
		{
			for (auto texture : Texture2DList)
			{
				VkDescriptorImageInfo DescriptorImage{};
				DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				DescriptorImage.imageView = texture->GetView();
				DescriptorImage.sampler = texture->GetSampler();
				DescriptorImageList.emplace_back(DescriptorImage);
			}
		}

		return DescriptorImageList;
	}

	static std::vector<std::shared_ptr<Texture2D>> GetTexture2DList()
	{
		return Texture2DList;
	}

	static std::shared_ptr<Texture2D> GetTexture2DByID(uint64_t TextureID)
	{
		uint64_t textureBufferIndex = -1;
		for (auto texture : Texture2DList)
		{
			if (texture->GetTextureID() == TextureID)
			{
				textureBufferIndex = texture->GetTextureBufferIndex();
				break;
			}
		}

		if (textureBufferIndex != -1)
		{
			return Texture2DList[textureBufferIndex];
		}

		return nullptr;
	}

	static std::shared_ptr<Texture2D> GetTexture2DByName(const std::string TextureName)
	{
		uint64_t textureBufferIndex = -1;
		for (auto texture : Texture2DList)
		{
			if (texture->GetTextureName() == TextureName)
			{
				textureBufferIndex = texture->GetTextureBufferIndex();
				break;
			}
		}

		if (textureBufferIndex != -1)
		{
			return Texture2DList[textureBufferIndex];
		}

		return nullptr;
	}

	static void Destroy()
	{
		vkDestroySampler(VulkanRenderer::GetDevice(), NullSampler, nullptr);
		NullSampler = VK_NULL_HANDLE;

		for (auto& texture : Texture2DList)
		{
			texture->Destroy();
		}
	}

	static void DestroyScene()
	{
		for (int x = Texture2DList.size() - 1; x >= 0; x--)
		{
			Texture2DList[x]->Destroy();
			Texture2DList.erase(Texture2DList.begin() + x);
		}
	}

	static void DestoryTexture(uint64_t TextureID)
	{
		std::shared_ptr<Texture2D> texture = GetTexture2DByID(TextureID);
		texture->Destroy();
		Texture2DList.erase(Texture2DList.begin() + texture->GetTextureBufferIndex());
	}
};