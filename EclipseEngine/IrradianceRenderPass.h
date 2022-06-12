#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class IrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	std::shared_ptr<GraphicsPipeline> irradiancePipeline;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};