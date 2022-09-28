#include "GaussianBlurRenderPass.h"

GaussianBlurRenderPass::GaussianBlurRenderPass() : RenderPass()
{
}

GaussianBlurRenderPass::~GaussianBlurRenderPass()
{
}

void GaussianBlurRenderPass::BuildRenderPass(std::vector<std::shared_ptr<RenderedColorTexture>> textureList)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    TextureMapMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(RenderPassResolution.x, RenderPassResolution.y)))) + 1;

    if (renderPass == nullptr)
    {
        DrawToBloomMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
        {
            BlurredTextureList.emplace_back(std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::vec2(static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip)), static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip))), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT)));
        }
    }
    else
    {
        DrawToBloomMap->RecreateRendererTexture(RenderPassResolution);
        for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
        {
            BlurredTextureList[mip]->RecreateRendererTexture(glm::vec2(static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip)), static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip))));
        }
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(DrawToBloomMap->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textureList);
    SetUpCommandBuffers();
}

void GaussianBlurRenderPass::OneTimeDraw(std::vector<std::shared_ptr<RenderedColorTexture>> textureList)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    TextureMapMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(RenderPassResolution.x, RenderPassResolution.y)))) + 1;

    if (renderPass == nullptr)
    {
        DrawToBloomMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
        {
            BlurredTextureList.emplace_back(std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::vec2(static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip)), static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip))), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT)));
        }
    }
    else
    {
        DrawToBloomMap->RecreateRendererTexture(RenderPassResolution);
        for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
        {
            BlurredTextureList[mip]->RecreateRendererTexture(glm::vec2(static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip)), static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip))));
        }
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(DrawToBloomMap->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textureList);
    SetUpCommandBuffers();
    Draw();
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void GaussianBlurRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(DrawToBloomMap->GetAttachmentDescription());

    std::vector<VkAttachmentReference> ColorRefsList;
    ColorRefsList.emplace_back(VkAttachmentReference{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = static_cast<uint32_t>(ColorRefsList.size());
    subpassDescription.pColorAttachments = ColorRefsList.data();

    std::vector<VkSubpassDependency> DependencyList;

    VkSubpassDependency FirstDependency = {};
    FirstDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    FirstDependency.dstSubpass = 0;
    FirstDependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    FirstDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    FirstDependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    FirstDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    FirstDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    DependencyList.emplace_back(FirstDependency);

    VkSubpassDependency SecondDependency = {};
    SecondDependency.srcSubpass = 0;
    SecondDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
    SecondDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    SecondDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    SecondDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    SecondDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    SecondDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    DependencyList.emplace_back(SecondDependency);

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(AttachmentDescriptionList.size());
    renderPassInfo.pAttachments = AttachmentDescriptionList.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(DependencyList.size());
    renderPassInfo.pDependencies = DependencyList.data();

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &renderPass))
    {
        throw std::runtime_error("Failed to create Buffer RenderPass.");
    }
}

void GaussianBlurRenderPass::BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedColorTexture>> textureList)
{
    VkPipelineColorBlendAttachmentState ColorAttachment;
    ColorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment.blendEnable = VK_FALSE;
    ColorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    ColorAttachmentList.clear();
    ColorAttachmentList.resize(1, ColorAttachment);

    PipelineInfoStruct pipelineInfo{};
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.ColorAttachments = ColorAttachmentList;
    pipelineInfo.SampleCount = SampleCount;

    blurPipeline.InitializePipeline(pipelineInfo, textureList);
}

VkCommandBuffer GaussianBlurRenderPass::Draw()
{
    if (DrawToBloomMap->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
        BlurredTextureList[0]->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        DrawToBloomMap->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
        {
            BlurredTextureList[mip]->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0);
        }
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkCommandBuffer commandBuffer = CommandBuffer[VulkanRenderer::GetCMDIndex()];
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
    {
        BlurredTextureList[mip]->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    }

    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = RenderPassFramebuffer[VulkanRenderer::GetCMDIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = { (uint32_t)RenderPassResolution.x, (uint32_t)RenderPassResolution.y };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    VkRect2D rect2D{};
    rect2D.extent.width = RenderPassResolution.x;
    rect2D.extent.height = RenderPassResolution.y;
    rect2D.offset.x = 0.0f;
    rect2D.offset.y = 0.0f;

    for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
    {
        VkViewport viewport{};
        viewport.width = static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip));
        viewport.height = static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip));
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        blurPipeline.Draw(commandBuffer);
        vkCmdEndRenderPass(commandBuffer);

        DrawToBloomMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Texture::CopyMipLevelToTexture(commandBuffer, DrawToBloomMap, BlurredTextureList[mip]);
        DrawToBloomMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
    {
        BlurredTextureList[mip]->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }


    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    return commandBuffer;
}

void GaussianBlurRenderPass::Destroy()
{
    DrawToBloomMap->Destroy();
    for (unsigned int mip = 0; mip < TextureMapMipLevels; mip++)
    {
        BlurredTextureList[mip]->Destroy();
    }

    blurPipeline.Destroy();
    RenderPass::Destroy();
}