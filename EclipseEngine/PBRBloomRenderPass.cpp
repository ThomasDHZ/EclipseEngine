#include "PBRBloomRenderPass.h"
#include "SkyboxMesh.h"


PBRBloomRenderPass::PBRBloomRenderPass() : RenderPass()
{
}

PBRBloomRenderPass::~PBRBloomRenderPass()
{
}

void PBRBloomRenderPass::BuildRenderPass(PBRRenderPassTextureSubmitList& textures)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    CubeMapMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(RenderPassResolution.x, RenderPassResolution.y)))) + 1;

    if (renderPass == nullptr)
    {
        DrawToCubeMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::ivec2(RenderPassResolution.x), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        PrefilterCubeMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::ivec2(RenderPassResolution.x), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT, CubeMapMipLevels));
        DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
    }
    else
    {
        DrawToCubeMap->RecreateRendererTexture(RenderPassResolution);
        PrefilterCubeMap->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(DrawToCubeMap->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
}

void PBRBloomRenderPass::OneTimeDraw(PBRRenderPassTextureSubmitList& textures)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    CubeMapMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(RenderPassResolution.x, RenderPassResolution.y)))) + 1;

    if (renderPass == nullptr)
    {
        DrawToCubeMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::ivec2(RenderPassResolution.x), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        PrefilterCubeMap = std::make_shared<RenderedColorTexture>(RenderedColorTexture(glm::ivec2(RenderPassResolution.x), VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT, CubeMapMipLevels));
        DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
    }
    else
    {
        DrawToCubeMap->RecreateRendererTexture(RenderPassResolution);
        PrefilterCubeMap->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(DrawToCubeMap->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
    Draw();
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void PBRBloomRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(DrawToCubeMap->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(DepthTexture->GetAttachmentDescription());

    std::vector<VkAttachmentReference> ColorRefsList;
    ColorRefsList.emplace_back(VkAttachmentReference{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = static_cast<uint32_t>(ColorRefsList.size());
    subpassDescription.pColorAttachments = ColorRefsList.data();
    subpassDescription.pDepthStencilAttachment = &depthReference;

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

void PBRBloomRenderPass::BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures)
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

    pbrBloomPipeline.InitializePipeline(pipelineInfo, textures);
}

VkCommandBuffer PBRBloomRenderPass::Draw()
{
    if (DrawToCubeMap->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
        PrefilterCubeMap->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        DrawToCubeMap->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        PrefilterCubeMap->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0);
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkCommandBuffer commandBuffer = CommandBuffer[VulkanRenderer::GetCMDIndex()];
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    PrefilterCubeMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0);

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {1.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

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

    PrefilterSkyboxSettings prefiliter;
    for (unsigned int mip = 0; mip < CubeMapMipLevels; mip++)
    {
        VkViewport viewport{};
        viewport.width = static_cast<float>(RenderPassResolution.x * std::pow(0.5f, mip));
        viewport.height = static_cast<float>(RenderPassResolution.y * std::pow(0.5f, mip));
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        prefiliter.SkyboxSize = RenderPassResolution.x;
        prefiliter.roughness = (float)mip / (float)(CubeMapMipLevels - 1);

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        {
            for (auto& mesh : MeshRendererManager::GetMeshList())
            {
                switch (mesh->GetMeshType())
                {
                case MeshTypeEnum::kPolygon:
                {
                    pbrBloomPipeline.Draw(commandBuffer, mesh);
                    break;
                }
                }
            }
        }
        vkCmdEndRenderPass(commandBuffer);

        DrawToCubeMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Texture::CopyTexture(commandBuffer, DrawToCubeMap, PrefilterCubeMap, mip);
        DrawToCubeMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    PrefilterCubeMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    return commandBuffer;
}

void PBRBloomRenderPass::Destroy()
{
    DrawToCubeMap->Destroy();
    PrefilterCubeMap->Destroy();
    pbrBloomPipeline.Destroy();
    RenderPass::Destroy();
}