#include "FrameBufferRenderPass.h"

FrameBufferRenderPass::FrameBufferRenderPass() : RenderPass()
{
}

FrameBufferRenderPass::~FrameBufferRenderPass()
{
}

void FrameBufferRenderPass::BuildRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
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

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void FrameBufferRenderPass::BuildRendererFramebuffers()
{
    RenderPassFramebuffer.resize(VulkanRenderer::GetSwapChainImageCount());

    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++) 
    {
        std::vector<VkImageView> AttachmentList;
        AttachmentList.emplace_back(VulkanRenderer::GetSwapChainImageViews()[x]);

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(AttachmentList.size());
        framebufferInfo.pAttachments = AttachmentList.data();
        framebufferInfo.width = RenderPassResolution.x;
        framebufferInfo.height = RenderPassResolution.y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &framebufferInfo, nullptr, &RenderPassFramebuffer[x]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }


}

void FrameBufferRenderPass::BuildRenderPassPipelines()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    ColorAttachmentList.clear();
    ColorAttachmentList.emplace_back(colorBlendAttachment);

    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/FrameBufferVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/FrameBufferFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;

        VkDescriptorImageInfo RenderedTextureBufferInfo = AddTextureDescriptor(RenderedTexture->View, RenderedTexture->Sampler);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 0, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
        buildGraphicsPipelineInfo.IncludeVertexDescriptors = false;

        if (frameBufferPipeline == nullptr)
        {
            frameBufferPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            frameBufferPipeline->Destroy();
            frameBufferPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
}

void FrameBufferRenderPass::StartUp(std::shared_ptr<RenderedColorTexture> renderedTexture)
{
    RenderedTexture = renderedTexture;

    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    BuildRenderPass();
    BuildRendererFramebuffers();
    BuildRenderPassPipelines();
    SetUpCommandBuffers();
}

void FrameBufferRenderPass::RebuildSwapChain(std::shared_ptr<RenderedColorTexture> renderedTexture)
{
    RenderedTexture = renderedTexture;

    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    vkDestroyRenderPass(VulkanRenderer::GetDevice(), renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : RenderPassFramebuffer)
    {
        vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }

    RenderPass::Destroy();

    BuildRenderPass();
    BuildRendererFramebuffers();
    BuildRenderPassPipelines();
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

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)RenderPassResolution.x;
    viewport.height = (float)RenderPassResolution.y;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D rect2D{};
    rect2D.offset = { 0, 0 };
    rect2D.extent = { (uint32_t)RenderPassResolution.x, (uint32_t)RenderPassResolution.y };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = RenderPassFramebuffer[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = Rect2D.offset;
    renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &CommandBufferBeginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }

    vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &viewport);
    vkCmdSetScissor(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &rect2D);
    vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline->GetShaderPipelineLayout(), 0, 1, frameBufferPipeline->GetDescriptorSetPtr(), 0, nullptr);
    vkCmdDraw(CommandBuffer[VulkanRenderer::GetCMDIndex()], 4, 1, 0, 0);
    vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }
}

void FrameBufferRenderPass::Destroy()
{
    frameBufferPipeline->Destroy();
    RenderPass::Destroy();
}