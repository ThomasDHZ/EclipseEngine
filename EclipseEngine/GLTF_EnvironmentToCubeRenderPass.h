#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "GLTF_EnvironmentToCubePipeline.h"
#include "JsonGraphicsPipeline.h"

class GLTF_EnvironmentToCubeRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

	JsonGraphicsPipeline EnvironmentToCubeRenderPassPipeline;

public:
	GLTF_EnvironmentToCubeRenderPass();
	~GLTF_EnvironmentToCubeRenderPass();

	void BuildRenderPass(uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void OneTimeDraw(uint32_t cubeMapSize);
	void Destroy();
};