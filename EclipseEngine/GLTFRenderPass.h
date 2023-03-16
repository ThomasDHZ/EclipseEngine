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

		GLTFPBRRenderPIpeline pbrPipeline;
		GLTF_SkyboxPipeline skyboxPipeline;

		void RenderPassDesc();
		void BuildRenderPassPipelines(std::shared_ptr<ModelRenderer> model);

public:
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::shared_ptr<ModelRenderer> model);
	VkCommandBuffer Draw(std::shared_ptr<ModelRenderer> model);
	void Destroy();
};

