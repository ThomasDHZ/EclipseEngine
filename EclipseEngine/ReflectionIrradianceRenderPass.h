#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class ReflectionIrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	ReflectionIrradianceRenderPass();
	~ReflectionIrradianceRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	std::shared_ptr<GraphicsPipeline> irradiancePipeline;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};

