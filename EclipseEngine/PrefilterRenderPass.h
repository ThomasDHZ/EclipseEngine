#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class PrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	PrefilterRenderPass();
	~PrefilterRenderPass();

	std::shared_ptr<GraphicsPipeline> prefilterPipeline;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};