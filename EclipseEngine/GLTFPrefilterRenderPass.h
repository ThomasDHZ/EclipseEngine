#pragma once
#include "RenderPass.h"
#include "RenderedCubeMapTexture.h"
#include "PrefilterPipeline.h"
#include "JsonGraphicsPipeline.h"

class GLTFPrefilterRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList);

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	JsonGraphicsPipeline PrefilterPipeline;

public:
	GLTFPrefilterRenderPass();
	~GLTFPrefilterRenderPass();


	std::vector<std::shared_ptr<RenderedCubeMapTexture>> PrefilterCubeMapList;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList, uint32_t cubeMapSize, glm::vec3 DrawPosition = glm::vec3(0.0f));
	VkCommandBuffer Draw(glm::vec3 DrawPosition = glm::vec3(0.0f));
	void Destroy();
};