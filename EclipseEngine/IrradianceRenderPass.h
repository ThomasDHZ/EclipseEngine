#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"

#include "JsonGraphicsPipeline.h"

class IrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	JsonGraphicsPipeline IrradiancePipeline;

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMap);

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	std::shared_ptr<RenderedCubeMapTexture> OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	std::shared_ptr<RenderedCubeMapTexture> DrawSubmit(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};