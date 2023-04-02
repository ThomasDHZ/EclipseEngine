#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "IrradiancePipeline.h"
#include "JsonGraphicsPipeline.h"

class IrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	JsonGraphicsPipeline IrradiancePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;

public:
	IrradianceRenderPass();
	~IrradianceRenderPass();

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> IrradianceCubeMapList;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};
