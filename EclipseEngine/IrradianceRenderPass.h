#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class IrradianceRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap);

	std::shared_ptr<Skybox> skybox;

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap;
	std::shared_ptr<GraphicsPipeline> irradiancePipeline;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize);
	void Draw();
	void Destroy();
};

