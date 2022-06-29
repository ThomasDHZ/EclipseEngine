#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class EnvironmentToCubeRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	EnvironmentToCubeRenderPass();
	~EnvironmentToCubeRenderPass();

	std::shared_ptr<GraphicsPipeline> EnvironmentToCubeRenderPassPipeline;

	void BuildRenderPass(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};

