#pragma once
#include "RenderedColorTexture.h"
#include "GLTFRenderPass.h"
#include "JsonGraphicsPipeline.h"
class PBRRenderPass : public GLTFRenderPass
{
private:
	JsonGraphicsPipeline PBRPipeline;
	JsonGraphicsPipeline PBRInstancePipeline;
	JsonGraphicsPipeline WireframePipeline;
	JsonGraphicsPipeline WireframeInstancePipeline;
	JsonGraphicsPipeline LinePipeline;
	JsonGraphicsPipeline SkyBoxPipeline;
	JsonGraphicsPipeline lightReflectionPipeline;

	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRRenderPass();
	~PBRRenderPass();


	void BuildRenderPass(std::string& renderPass, PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};
