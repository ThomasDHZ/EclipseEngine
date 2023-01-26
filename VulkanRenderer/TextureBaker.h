\\#pragma once
\\#include "VulkanRenderer.h"
\\#include "BakedTexture.h"
\\#include "RenderedCubeMapTexture.h"
\\
\\class TextureBaker
\\{
\\public:
\\	static void BakeCubeTextureAtlus(const std::string& FilePath, std::vector<std::shared_ptr<RenderedCubeMapTexture>> ReflectionCubeMapList, uint32_t cubeMapSizePerCell, uint32_t bakedTextureAtlusSize);
\\};
\\
