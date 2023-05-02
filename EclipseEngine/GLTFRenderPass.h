#pragma once
#include "GLTFPBRRenderPIpeline.h"
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "GLTF_SkyboxPipeline.h"
#include "WireFramePipeline.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "PBRInstancePipeline.h"
class GLTFRenderPass : public RenderPass
{
	private:
		std::shared_ptr<RenderedColorTexture> ColorTexture;
		std::shared_ptr<RenderedDepthTexture> DepthTexture;

		GLTFPBRRenderPIpeline oldpbrPipeline;
		PBRInstancePipeline oldinstancedPipeline;
		WireFramePipeline oldwireframePipeline;
		LinePipeline oldLinePipeline;

		JsonGraphicsPipeline PBRPipeline;
		JsonGraphicsPipeline PBRInstancePipeline;
		JsonGraphicsPipeline WireframePipeline;
		JsonGraphicsPipeline WireframeInstancePipeline;
		JsonGraphicsPipeline LinePipeline;
		JsonGraphicsPipeline SkyBoxPipeline;

		void RenderPassDesc();
		void BuildRenderPassPipelines(std::vector<std::shared_ptr<GameObject>>& gameObjectList);

public:
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::vector<std::shared_ptr<GameObject>>& gameObjectList);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<GameObject>>& gameObjectList);
	void Destroy();
};

