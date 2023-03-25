#pragma once
#include "GLTFPBRRenderPIpeline.h"
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "GLTF_SkyboxPipeline.h"
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
		JsonGraphicsPipeline pbrPipeline;
		JsonGraphicsPipeline pbrPipeline2;
		GLTF_SkyboxPipeline skyboxPipeline;

		void RenderPassDesc();
		void BuildRenderPassPipelines(std::vector<GLTF_Temp_Model> modelList);

public:
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::vector<GLTF_Temp_Model> modelList);
	VkCommandBuffer Draw(std::vector<GLTF_Temp_Model> modelList);
	void Destroy();
};

