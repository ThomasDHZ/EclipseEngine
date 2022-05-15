#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class PrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;
	std::shared_ptr<Skybox> skybox;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	bool firstRun = true;

public:
	PrefilterRenderPass();
	~PrefilterRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap;
	std::shared_ptr<GraphicsPipeline> prefilterPipeline;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};