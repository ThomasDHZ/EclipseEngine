#include "FrameBufferRenderPass.h"

FrameBufferRenderPass::FrameBufferRenderPass() : BaseRenderPass()
{
}

FrameBufferRenderPass::~FrameBufferRenderPass()
{
}

void FrameBufferRenderPass::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 1> attachments = { colorAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void FrameBufferRenderPass::CreateRendererFramebuffers()
{
    SwapChainFramebuffers.resize(VulkanRenderer::GetSwapChainImageCount());

    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++) 
    {
        std::array<VkImageView, 1> attachments =
        {
            VulkanRenderer::GetSwapChainImageViews()[x]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = RenderPassResolution.x;
        framebufferInfo.height = RenderPassResolution.y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &framebufferInfo, nullptr, &SwapChainFramebuffers[x]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }


}

void FrameBufferRenderPass::StartUp(std::shared_ptr<RenderedColorTexture> RenderedTexture)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    CreateRenderPass();
    CreateRendererFramebuffers();
    frameBufferPipeline = std::make_shared<FrameBufferPipeline>(FrameBufferPipeline(RenderPass, RenderedTexture));
    SetUpCommandBuffers();
}

void FrameBufferRenderPass::RebuildSwapChain(std::shared_ptr<RenderedColorTexture> RenderedTexture)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    frameBufferPipeline->Destroy();

    vkDestroyRenderPass(VulkanRenderer::GetDevice(), RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }

    CreateRenderPass();
    CreateRendererFramebuffers();
    frameBufferPipeline->UpdateGraphicsPipeLine(RenderPass, RenderedTexture);
    SetUpCommandBuffers();
}

void FrameBufferRenderPass::Draw()
{
    VkCommandBufferBeginInfo CommandBufferBeginInfo{};
    CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRect2D Rect2D{};
    Rect2D.offset.x = 0;
    Rect2D.offset.y = 0;

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &CommandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = Rect2D.offset;
    renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline->GetShaderPipelineLayout(), 0, 1, frameBufferPipeline->GetDescriptorSetPtr(), 0, nullptr);
    vkCmdDraw(CommandBuffer[VulkanRenderer::GetCMDIndex()], 4, 1, 0, 0);
    vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FrameBufferRenderPass::Destroy()
{
    frameBufferPipeline->Destroy();
    BaseRenderPass::Destroy();
}