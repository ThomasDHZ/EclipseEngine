#include "DepthRenderPass.h"

DepthRenderPass::DepthRenderPass() : RenderPass()
{
}

DepthRenderPass::~DepthRenderPass()
{
}

//void DepthRenderPass::BuildRenderPass(std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList, glm::vec2 TextureResolution)
//{
//    SampleCount = VK_SAMPLE_COUNT_1_BIT;
//    RenderPassResolution = TextureResolution;
//
//    if (renderPass == nullptr)
//    {
//        RenderPassDepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));
//        for (auto& light : DirectionalLightList)
//        {
//            DepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount)));
//        }
//    }
//    else
//    {
//        ClearTextureList();
//        RenderPassDepthTexture->RecreateRendererTexture(RenderPassResolution);
//        for (auto& light : DirectionalLightList)
//        {
//            DepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount)));
//        }
//        RenderPass::Destroy();
//    }
//
//
//    std::vector<VkImageView> AttachmentList;
//    AttachmentList.emplace_back(RenderPassDepthTexture->View);
//
//    RenderPassDesc();
//    CreateRendererFramebuffers(AttachmentList);
//    BuildRenderPassPipelines();
//    SetUpCommandBuffers();
//}
//
//void DepthRenderPass::OneTimeDraw(std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList, glm::vec2 TextureResolution)
//{
//    SampleCount = VK_SAMPLE_COUNT_1_BIT;
//    RenderPassResolution = TextureResolution;
//
//    if (renderPass == nullptr)
//    {
//        RenderPassDepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));
//        for (auto& light : DirectionalLightList)
//        {
//            DepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount)));
//        }
//    }
//    else
//    {
//        ClearTextureList();
//        RenderPassDepthTexture->RecreateRendererTexture(RenderPassResolution);
//        for (auto& light : DirectionalLightList)
//        {
//            DepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount)));
//        }
//        RenderPass::Destroy();
//    }
//
//    std::vector<VkImageView> AttachmentList;
//    AttachmentList.emplace_back(RenderPassDepthTexture->View);
//
//    RenderPassDesc();
//    CreateRendererFramebuffers(AttachmentList);
//    BuildRenderPassPipelines();
//    SetUpCommandBuffers();
//    Draw();
//    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
//}


void DepthRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(RenderPassDepthTexture->GetAttachmentDescription());


    VkAttachmentReference depthReference = { 0, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
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

void DepthRenderPass::BuildRenderPassPipelines()
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

    depthPipeline.InitializePipeline(pipelineInfo);
    depthInstancedPipeline.InitializePipeline(pipelineInfo);
}

void DepthRenderPass::ClearTextureList()
{
    for (auto& depthTexture : DepthTextureList)
    {
        depthTexture->Destroy();
    }
    DepthTextureList.clear();
}

VkCommandBuffer DepthRenderPass::Draw()
{

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].depthStencil = { 1.0f, 0 };

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
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = { (uint32_t)RenderPassResolution.x, (uint32_t)RenderPassResolution.y };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    VkCommandBuffer commandBuffer = CommandBuffer[VulkanRenderer::GetCMDIndex()];
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }
    for (int x = 0; x < DepthTextureList.size(); x++)
    {
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);
        //{
        //    for (auto& mesh : MeshRendererManager::GetMeshList())
        //    {
        //        switch (mesh->GetMeshType())
        //        {
        //        case MeshTypeEnum::kMeshPolygon:
        //        {
        //            depthPipeline.Draw(commandBuffer, mesh, x);
        //            break;
        //        }
        //        case MeshTypeEnum::kMeshPolygonInstanced:
        //        {
        //            depthInstancedPipeline.Draw(commandBuffer, mesh, x);
        //            break;
        //        }
        //        default:
        //        {
        //            break;
        //        }
        //        }
        //    }
        //}
        vkCmdEndRenderPass(commandBuffer);

        RenderPassDepthTexture->UpdateDepthImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        DepthTextureList[x]->UpdateDepthImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Texture::CopyDepthTexture(commandBuffer, RenderPassDepthTexture, DepthTextureList[x]);
        DepthTextureList[x]->UpdateDepthImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        RenderPassDepthTexture->UpdateDepthImageLayout(commandBuffer, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }

    return commandBuffer;
}

void DepthRenderPass::Destroy()
{
    ClearTextureList();
    for (auto& depthTexture : DepthTextureList)
    {
        depthTexture->Destroy();
    }
    RenderPassDepthTexture->Destroy();
    depthPipeline.Destroy();
    depthInstancedPipeline.Destroy();
    RenderPass::Destroy();
}