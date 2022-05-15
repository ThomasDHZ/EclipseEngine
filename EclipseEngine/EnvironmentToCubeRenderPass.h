#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class EnvironmentToCubeRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

	std::shared_ptr<Skybox> skybox;

public:
	EnvironmentToCubeRenderPass();
	~EnvironmentToCubeRenderPass();

	std::shared_ptr<GraphicsPipeline> EnvironmentToCubeRenderPassPipeline;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};

