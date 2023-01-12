#pragma once
#include <memory>
#include <vector>
#include "Material.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "EnvironmentTexture.h"
#include "TextureAtlus.h"

class VRAMManager
{
private:
	static std::shared_ptr<Material> DefaultMaterial;
	static std::vector<std::shared_ptr<Material>> MaterialList;
	static std::vector<std::shared_ptr<Texture2D>> Texture2DList;
	static VkSampler NullSampler;

	static void UpdateBufferIndex();
	static std::shared_ptr<Texture2D> IsTexture2DLoaded(std::string name);

public:

	static void StartUp();
	static std::shared_ptr<Texture2D> LoadTexture2D(std::shared_ptr<Texture2D> texture);
	static std::shared_ptr<Texture2D> LoadTexture2D(const std::string TextureLocation, TextureTypeEnum textureType, VkFormat format);
	static std::shared_ptr<TextureAtlus> LoadTextureAtlus(const std::string& TextureLocation, const glm::ivec2& TextureCellSize);
	
	static uint64_t AddDefaultMaterial();
	static void AddMaterial(const std::shared_ptr<Material> material);

	static std::shared_ptr<Texture2D> GetTexture2DByID(uint64_t TextureID);
	static std::shared_ptr<Texture2D> GetTexture2DByName(const std::string TextureName);
	static std::shared_ptr<Material> GetMaterialByID(uint64_t MaterialID);

	static std::vector<std::shared_ptr<Material>> GetMaterialList() { return  MaterialList; }
};

