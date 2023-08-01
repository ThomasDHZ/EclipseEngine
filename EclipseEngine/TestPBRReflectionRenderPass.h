#pragma once
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"

class TestPBRReflectionRenderPass : public RenderPass
{
private:


	JsonGraphicsPipeline PBRPipeline;
	//JsonGraphicsPipeline PBRInstancePipeline;
	//JsonGraphicsPipeline WireframePipeline;
	//JsonGraphicsPipeline WireframeInstancePipeline;
	//JsonGraphicsPipeline LinePipeline;
	JsonGraphicsPipeline SkyBoxPipeline;
	//JsonGraphicsPipeline lightReflectionPipeline;


	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	TestPBRReflectionRenderPass();
	~TestPBRReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;
	CubeMapSamplerBuffer ReflectionMapSampler;

	void UpdateView(glm::vec3 reflectPoint);

	void RenderPassDesc();
	void BuildRenderPass(std::string& renderPass, PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw(glm::vec3 reflectPoint);
	void Destroy();
};

