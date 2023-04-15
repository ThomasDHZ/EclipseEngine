#pragma once
#include <memory>
#include <vector>
#include "GLTFMaterial.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "EnvironmentTexture.h"
#include "TextureAtlus.h"

class VRAMManager
{
private:
	static std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;
	static std::vector<std::shared_ptr<Texture2D>> Texture2DList;
	static VkSampler NullSampler;

	static void UpdateBufferIndex();
	static std::shared_ptr<Texture2D> IsTexture2DLoaded(std::string name);
	static std::shared_ptr<Texture2D> IsTexture2DLoaded(const TinyGltfTextureLoader& textureLoader);

public:

	static void StartUp();
	static std::shared_ptr<Texture2D> LoadTexture2D(std::shared_ptr<Texture2D> texture);
	static std::shared_ptr<Texture2D> LoadTexture2D(const TinyGltfTextureLoader& textureLoader, const TinyGltfTextureSamplerLoader& samplerLoader, VkFormat format, TextureTypeEnum textureType);
	static std::shared_ptr<TextureAtlus> LoadTextureAtlus(const std::string& TextureLocation, const glm::ivec2& TextureCellSize);
	
	static uint64_t AddDefaultMaterial();
	static void AddMaterial(const std::shared_ptr<GLTFMaterial> material);

	static std::shared_ptr<Texture2D> GetTexture2DByID(uint64_t TextureID);
	static std::shared_ptr<Texture2D> GetTexture2DByName(const std::string TextureName);
	static std::shared_ptr<GLTFMaterial> GetMaterialByID(uint64_t MaterialID);

	static std::vector<std::shared_ptr<GLTFMaterial>> GetMaterialList() { return  MaterialList; }
};

