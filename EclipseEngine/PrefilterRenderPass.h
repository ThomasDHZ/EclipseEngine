#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class PrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	PrefilterRenderPass();
	~PrefilterRenderPass();


	std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	std::shared_ptr<GraphicsPipeline> prefilterPipeline;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap, uint32_t cubeMapSize);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};