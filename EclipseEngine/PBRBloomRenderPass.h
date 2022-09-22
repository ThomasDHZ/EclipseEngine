#pragma once
#include "RenderPass.h"
#include "PBRBloomPipeline.h"

class PBRBloomRenderPass : public RenderPass
{
private:
	uint32_t CubeMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

	std::shared_ptr<RenderedColorTexture> DrawToCubeMap;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;
	PBRBloomPipeline pbrBloomPipeline;

public:
	PBRBloomRenderPass();
	~PBRBloomRenderPass();


	std::shared_ptr<RenderedColorTexture> PrefilterCubeMap;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void OneTimeDraw(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, glm::vec3 DrawPosition = glm::vec3(0.0f));
	VkCommandBuffer Draw(glm::vec3 DrawPosition = glm::vec3(0.0f));
	void Destroy();
};