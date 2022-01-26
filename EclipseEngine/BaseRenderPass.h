#pragma once
#include "VulkanRenderer.h"

class BaseRenderPass
{
protected:
	VkRenderPass RenderPass = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> CommandBuffer;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	glm::ivec2 RenderPassResolution;

public:

	BaseRenderPass();
	~BaseRenderPass();

	void OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer);
	virtual void SetUpCommandBuffers();
	virtual void Destroy();

	VkCommandBuffer GetCommandBuffer() { return CommandBuffer[VulkanRenderer::GetCMDIndex()]; }
};
