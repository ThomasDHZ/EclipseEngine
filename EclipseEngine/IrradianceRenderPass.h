#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class IrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	std::shared_ptr<GraphicsPipeline> irradiancePipeline;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap, uint32_t cubeMapSize);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> reflectionCubeMap, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};