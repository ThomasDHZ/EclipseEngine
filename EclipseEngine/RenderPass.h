#pragma once
#include "VulkanRenderer.h"
#include "JsonGraphicsPipeline.h"
#include <fstream>

class RenderPass
{
private:
	std::vector<JsonGraphicsPipeline> RendererPipelineList;
	std::vector<RenderedColorTexture> ColorTextureList;
	std::vector<RenderedDepthTexture> DepthTextureList;

	VkShaderModule ReadShaderFile(const std::string& filename);

protected:
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

public:

	RenderPass();
	~RenderPass();

	void OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer);
	virtual void SetUpCommandBuffers();
	virtual void Destroy();

	VkCommandBuffer GetCommandBuffer() { return CommandBuffer[VulkanRenderer::GetCMDIndex()]; }
};

