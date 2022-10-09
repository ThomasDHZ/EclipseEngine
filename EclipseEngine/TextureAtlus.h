#pragma once
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "ReadableTexture.h"

class TextureAtlus : public Texture2D
{
	private:
		glm::ivec2 TextureCellSize = glm::ivec2(0.0f);

	public:
		TextureAtlus();
		TextureAtlus(const std::string& textureLocation, const glm::ivec2& textureCellSize);
		~TextureAtlus();

		void CopyCellToTexture(std::shared_ptr<Texture> texture, const glm::ivec2& cellToTransfer);
		void CopyCellToCubeMapSide(std::shared_ptr<CubeMapTexture> texture, const glm::ivec2& cellToTransfer);

		std::shared_ptr<Texture> CreateTextureFromTextureAtlus(const glm::ivec2& cellToTransfer);
		std::shared_ptr<CubeMapTexture> CreateCubeMapTextureFromTextureAtlus(VkCommandBuffer& commandBuffer);
};
