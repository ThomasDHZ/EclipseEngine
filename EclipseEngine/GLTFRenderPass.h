#pragma once
#include "GLTFPBRRenderPIpeline.h"
#include "RenderedColorTexture.h"
#include "RenderPass.h"

class GLTFRenderPass : public RenderPass
{
	private:
		std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
		std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
		VkVertexInputBindingDescription VertexInputBindingDescription;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

		std::shared_ptr<RenderedColorTexture> ColorTexture;
		std::shared_ptr<RenderedDepthTexture> DepthTexture;

		GLTFPBRRenderPIpeline pbrPipeline;

		void RenderPassDesc();
		void BuildRenderPassPipelines(GLTF_Temp_Model& model);

public:
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(GLTF_Temp_Model& model);
	VkCommandBuffer Draw(GLTF_Temp_Model& model);
	void Destroy();
};

