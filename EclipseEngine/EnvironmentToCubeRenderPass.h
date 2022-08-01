#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "SkyboxMesh.h"
#include "EnvironmentToCubePipeline.h"

class EnvironmentToCubeRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	EnvironmentToCubeRenderPass();
	~EnvironmentToCubeRenderPass();

	EnvironmentToCubePipeline EnvironmentToCubeRenderPassPipeline;

	void BuildRenderPass(uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};

