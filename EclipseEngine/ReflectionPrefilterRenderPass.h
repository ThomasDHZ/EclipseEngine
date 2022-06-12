#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class ReflectionPrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	ReflectionPrefilterRenderPass();
	~ReflectionPrefilterRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	std::shared_ptr<GraphicsPipeline> prefilterPipeline;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};