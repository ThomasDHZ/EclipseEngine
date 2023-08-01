#pragma once
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"

class TestPBRCubeMapRenderPass : public RenderPass
{
private:


	JsonGraphicsPipeline pbrReflectionPipeline;
	JsonGraphicsPipeline skyboxPipeline;

	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	TestPBRCubeMapRenderPass();
	~TestPBRCubeMapRenderPass();

	std::shared_ptr<RenderedColorTexture> DrawToTexture;
	std::shared_ptr<RenderedDepthTexture> DrawToDepthTexture;
	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	CubeMapSamplerBuffer ReflectionMapSampler;

	void UpdateView(glm::vec3 reflectPoint);
	void RenderPassDesc();
	void BuildRenderPass(std::string& renderPass, PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw(glm::vec3 reflectPoint);
	void Destroy();
};
