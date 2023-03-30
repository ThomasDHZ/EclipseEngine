#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "CubeToEnvironmentPipeline.h"

class CubeToEnvironmentRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMap);

public:
	CubeToEnvironmentRenderPass();
	~CubeToEnvironmentRenderPass();

	std::shared_ptr<RenderedColorTexture> environmentMap;
	CubeToEnvironmentPipeline cubeToEnvironmentPipeline;

	void BuildRenderPass(const glm::vec2& environmentMapSize, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	void Draw();
	void Destroy();
};

