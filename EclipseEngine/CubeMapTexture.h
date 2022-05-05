#pragma once
#include "Texture.h"

struct CubeMapLayout
{
	std::string Front;
	std::string Back;
	std::string Top;
	std::string Bottom;
	std::string Right;
	std::string Left;
};

class CubeMapTexture : public Texture
{
private:
	void LoadTexture(CubeMapLayout CubeMapFiles, VkFormat textureFormat);
	void CreateTextureView(VkFormat textureFormat);
	void CreateTextureSampler();
public:
	CubeMapTexture();
	CubeMapTexture(CubeMapLayout CubeMapFiles);
	CubeMapTexture(glm::ivec2 TextureResolution, TextureTypeEnum textureType);
	~CubeMapTexture();
};