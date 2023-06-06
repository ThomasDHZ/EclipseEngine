#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"

#include "JsonGraphicsPipeline.h"

class IrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	JsonGraphicsPipeline IrradiancePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMap);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	void OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};