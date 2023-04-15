#include "VRAMManager.h"

 //std::shared_ptr<Material> VRAMManager::DefaultMaterial;
 std::vector<std::shared_ptr<GLTFMaterial>> VRAMManager::MaterialList;
 std::vector<std::shared_ptr<Texture2D>> VRAMManager::Texture2DList;
 VkSampler VRAMManager::NullSampler = VK_NULL_HANDLE;

void VRAMManager::UpdateBufferIndex()
{
	for (int x = 0; x < Texture2DList.size(); x++)
	{
		Texture2DList[x]->UpdateTextureBufferIndex(x);
	}

	for (int x = 0; x < MaterialList.size(); x++)
	{
		MaterialList[x]->UpdateMaterialBufferIndex(x);
	}
}

std::shared_ptr<Texture2D> VRAMManager::IsTexture2DLoaded(const TinyGltfTextureLoader& textureLoader)
{
	uint64_t textureID = 0;
	for (auto texture : Texture2DList)
	{
		if (texture->GetFilePath() == textureLoader.uri)
		{
			return texture;
		}
	}

	return nullptr;
}

std::shared_ptr<Texture2D> VRAMManager::IsTexture2DLoaded(std::string name)
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

void VRAMManager::StartUp()
{
	//LoadTexture2D("../texture/DefaultTexture.png", TextureTypeEnum::kDiffuseTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	//LoadTexture2D("../texture/AlphaDefault.png", TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM);

	//VkSamplerCreateInfo NullSamplerInfo = {};
	//NullSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	//NullSamplerInfo.magFilter = VK_FILTER_NEAREST;
	//NullSamplerInfo.minFilter = VK_FILTER_NEAREST;
	//NullSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	//NullSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	//NullSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	//NullSamplerInfo.anisotropyEnable = VK_TRUE;
	//NullSamplerInfo.maxAnisotropy = 16.0f;
	//NullSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	//NullSamplerInfo.unnormalizedCoordinates = VK_FALSE;
	//NullSamplerInfo.compareEnable = VK_FALSE;
	//NullSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	//NullSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	//NullSamplerInfo.minLod = 0;
	//NullSamplerInfo.maxLod = 0;
	//NullSamplerInfo.mipLodBias = 0;

	//if (vkCreateSampler(VulkanRenderer::GetDevice(), &NullSamplerInfo, nullptr, &NullSampler))
	//{
	//	throw std::runtime_error("Failed to create Sampler.");
	//}

	//uint64_t materialID = AddDefaultMaterial();
	//DefaultMaterial = GetMaterialByID(materialID);
}

std::shared_ptr<Texture2D> VRAMManager::LoadTexture2D(std::shared_ptr<Texture2D> texture)
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

std::shared_ptr<Texture2D> VRAMManager::LoadTexture2D(const TinyGltfTextureLoader& textureLoader, const TinyGltfTextureSamplerLoader& samplerLoader, VkFormat format, TextureTypeEnum textureType)
{
	std::shared_ptr<Texture2D> isTextureLoaded = IsTexture2DLoaded(textureLoader);
	if (isTextureLoaded != nullptr)
	{
		return isTextureLoaded;
	}

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(Texture2D(textureLoader, samplerLoader, format, textureType));
	Texture2DList.emplace_back(texture);
	Texture2DList.back()->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
	return texture;
}

void VRAMManager::AddMaterial(const std::shared_ptr<GLTFMaterial> material)
{
	MaterialList.emplace_back(material);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
}