#pragma once
#include "VulkanRenderer.h"
#include "JsonGraphicsPipeline.h"

#include <fstream>

struct RenderPassInput
{
	std::vector<JsonGraphicsPipeline> RendererPipelineList;
	std::vector<std::shared_ptr<RenderedColorTexture>> MultiSampledColorTextureList;
	std::vector<std::shared_ptr<RenderedColorTexture>> ColorTextureList;
	std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;
};

class GLTFRenderPass
{
private:
	VkShaderModule ReadShaderFile(const std::string& filename);

protected:
	std::vector<JsonGraphicsPipeline> RendererPipelineList;
	std::vector<std::shared_ptr<RenderedColorTexture>> MultiSampledColorTextureList;
	std::vector<std::shared_ptr<RenderedColorTexture>> ColorTextureList;
	std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;

	glm::ivec2 RenderPassResolution;

	VkRenderPass renderPass = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> CommandBuffer;
	std::vector<VkFramebuffer> RenderPassFramebuffer;

	VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
	virtual void CreateRendererFramebuffers(std::vector<VkImageView>& AttachmentList);
	virtual void RenderPassDesc();
	virtual void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:

	GLTFRenderPass();
	~GLTFRenderPass();

	std::vector<std::shared_ptr<RenderedColorTexture>> RenderedTexture;

	virtual void OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer);
	virtual void SetUpCommandBuffers();
	virtual void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	virtual void Destroy();
	virtual VkCommandBuffer Draw();

	VkCommandBuffer GetCommandBuffer() { return CommandBuffer[VulkanRenderer::GetCMDIndex()]; }
};

