#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "JsonGraphicsPipeline.h"

class EnvironmentToCubeRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

	JsonGraphicsPipeline EnvironmentToCubeRenderPassPipeline;

public:
	EnvironmentToCubeRenderPass();
	~EnvironmentToCubeRenderPass();

	void BuildRenderPass(uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void OneTimeDraw(uint32_t cubeMapSize);
	void Destroy();
};