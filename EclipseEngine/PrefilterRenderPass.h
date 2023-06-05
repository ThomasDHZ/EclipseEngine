#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "JsonGraphicsPipeline.h"

class PrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture>& cubeMap);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	JsonGraphicsPipeline PrefilterPipeline;

public:
	PrefilterRenderPass();
	~PrefilterRenderPass();


	std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture>& cubeMap, uint32_t cubeMapSize);
	void OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture>& cubeMap, uint32_t cubeMapSize, glm::vec3 DrawPosition = glm::vec3(0.0f));
	VkCommandBuffer Draw(glm::vec3 DrawPosition = glm::vec3(0.0f));
	void Destroy();
};