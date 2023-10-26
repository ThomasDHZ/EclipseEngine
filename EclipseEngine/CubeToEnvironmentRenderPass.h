#pragma once
#include "GenerateImageRenderPass.h"

class CubeToEnvironmentRenderPass : public RenderPass
{
	virtual void RenderPassDesc();
	virtual void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMap);
protected:
	TextureCreatorProperties textureCreatorProperties;
	JsonGraphicsPipeline ImagePipeline;
	std::shared_ptr<RenderedColorTexture> ImageTexture;
public:
	CubeToEnvironmentRenderPass();
	~CubeToEnvironmentRenderPass();

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize);
	void OneTimeDraw( std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize);

	VkCommandBuffer Draw(float time);
	void Destroy();
	void SaveTexture(const char* filename);

	std::shared_ptr<RenderedColorTexture> GetImageTexture() { return ImageTexture; }
};

