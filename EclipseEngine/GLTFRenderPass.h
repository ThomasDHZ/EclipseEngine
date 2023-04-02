#pragma once
#include "GLTFPBRRenderPIpeline.h"
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "GLTF_SkyboxPipeline.h"
#include "WireFramePipeline.h"
#include "LinePipeline.h"
class GLTFRenderPass : public RenderPass
{
	private:
		std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
		std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
		VkVertexInputBindingDescription VertexInputBindingDescription;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

		std::shared_ptr<RenderedColorTexture> ColorTexture;
		std::shared_ptr<RenderedDepthTexture> DepthTexture;

		//GLTFPBRRenderPIpeline pbrPipeline;
		//GLTF_SkyboxPipeline skyboxPipeline;
		std::vector<JsonGraphicsPipeline> PBRPipelineList;
		std::vector<JsonGraphicsPipeline> WireframePipelineList;
		std::vector<JsonGraphicsPipeline> LinePipelineList;
		JsonGraphicsPipeline SkyBoxPipeline;

		void RenderPassDesc();
		void BuildRenderPassPipelines(std::vector<std::shared_ptr<GLTF_Temp_Model>> modelList);

public:
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::vector<std::shared_ptr<GLTF_Temp_Model>> modelList);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<GLTF_Temp_Model>> modelList);
	void Destroy();
};

