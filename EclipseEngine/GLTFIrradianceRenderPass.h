#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "SkyboxMesh.h"
#include "IrradiancePipeline.h"

class GLTFIrradianceRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	IrradiancePipeline irradiancePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;

public:
	GLTFIrradianceRenderPass();
	~GLTFIrradianceRenderPass();

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> IrradianceCubeMapList;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};
