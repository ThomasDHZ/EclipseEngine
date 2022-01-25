#include "BaseRenderPass.h"

BaseRenderPass::BaseRenderPass()
{

}

BaseRenderPass::~BaseRenderPass()
{
}

void BaseRenderPass::Destroy()
{
    vkDestroyRenderPass(VulkanRenderer::Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(VulkanRenderer::Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}

void BaseRenderPass::OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = CMDBuffer;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;

    VkFence fence;
    vkCreateFence(VulkanRenderer::Device, &fenceCreateInfo, nullptr, &fence);
    vkQueueSubmit(VulkanRenderer::GraphicsQueue, 1, &submitInfo, fence);
    vkWaitForFences(VulkanRenderer::Device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(VulkanRenderer::Device, fence, nullptr);
}

void BaseRenderPass::SetUpCommandBuffers()
{
    CommandBuffer.resize(VulkanRenderer::GetSwapChainImageCount());
    for (size_t i = 0; i < VulkanRenderer::GetSwapChainImageCount(); i++)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = VulkanRenderer::CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(VulkanRenderer::Device, &allocInfo, &CommandBuffer[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
}