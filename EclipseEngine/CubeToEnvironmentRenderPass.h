#pragma once
#include "GenerateImageRenderPass.h"

class CubeToEnvironmentRenderPass : public GenerateImageRenderPass
{
private:
public:
	CubeToEnvironmentRenderPass();
	~CubeToEnvironmentRenderPass();

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize);
	void OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize);
};

