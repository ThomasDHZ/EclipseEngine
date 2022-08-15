#include "PBRRenderPass.h"
#include "LightManager.h"


PBRRenderPass::PBRRenderPass() : RenderPass()
{
}

PBRRenderPass::~PBRRenderPass()
{
}

void PBRRenderPass::BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceTexture, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterTexture, std::shared_ptr<RenderedDepthTexture> depthTexture)
{
    SampleCount = GraphicsDevice::GetMaxSampleCount();
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    if (renderPass == nullptr)
    {
        ColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
        RenderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
        BloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
        RenderedBloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
        DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));
    }
    else
    {
        ColorTexture->RecreateRendererTexture(RenderPassResolution);
        RenderedTexture->RecreateRendererTexture(RenderPassResolution);
        BloomTexture->RecreateRendererTexture(RenderPassResolution);
        RenderedBloomTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(ColorTexture->View);
    AttachmentList.emplace_back(BloomTexture->View);
    AttachmentList.emplace_back(RenderedTexture->View);
    AttachmentList.emplace_back(RenderedBloomTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(reflectionIrradianceTexture, reflectionPrefilterTexture, depthTexture);
    SetUpCommandBuffers();
}

void PBRRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(ColorTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(BloomTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(RenderedTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(RenderedBloomTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(DepthTexture->GetAttachmentDescription());

    std::vector<VkAttachmentReference> ColorRefsList;
    ColorRefsList.emplace_back(VkAttachmentReference{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    std::vector<VkAttachmentReference> MultiSampleReferenceList;
    MultiSampleReferenceList.emplace_back(VkAttachmentReference{ 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    MultiSampleReferenceList.emplace_back(VkAttachmentReference{ 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkAttachmentReference depthReference = { 4, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = static_cast<uint32_t>(ColorRefsList.size());
    subpassDescription.pColorAttachments = ColorRefsList.data();
    subpassDescription.pDepthStencilAttachment = &depthReference;
    subpassDescription.pResolveAttachments = MultiSampleReferenceList.data();

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

void PBRRenderPass::BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceTexture, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterTexture, std::shared_ptr<RenderedDepthTexture> depthTexture)
{
    VkPipelineColorBlendAttachmentState ColorAttachment;
    ColorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment.blendEnable = VK_TRUE;
    ColorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    ColorAttachmentList.clear();
    ColorAttachmentList.emplace_back(ColorAttachment);
    ColorAttachmentList.emplace_back(ColorAttachment);

    PipelineInfoStruct pipelineInfo{};
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.ColorAttachments = ColorAttachmentList;
    pipelineInfo.SampleCount = SampleCount;

    pbrPipeline.InitializePipeline(pipelineInfo, reflectionIrradianceTexture, reflectionPrefilterTexture, depthTexture);
    skyboxPipeline.InitializePipeline(pipelineInfo, SceneManager::CubeMap);
    drawLinePipeline.InitializePipeline(pipelineInfo);
    wireframePipeline.InitializePipeline(pipelineInfo);
    outLinePipeline.InitializePipeline(pipelineInfo);
}

VkCommandBuffer PBRRenderPass::Draw()
{

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 5> clearValues{};
    clearValues[0].color = { {0.2f, 0.3f, 0.3f, 1.0f} };
    clearValues[1].color = { {0.2f, 0.3f, 0.3f, 1.0f} };
    clearValues[2].color = { {0.2f, 0.3f, 0.3f, 1.0f} };
    clearValues[3].color = { {0.2f, 0.3f, 0.3f, 1.0f} };
    clearValues[4].depthStencil = { 1.0f, 0 };

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
    renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    VkCommandBuffer commandBuffer = CommandBuffer[VulkanRenderer::GetCMDIndex()];
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);
    {
        skyboxPipeline.Draw(commandBuffer);

        for (auto& mesh : MeshRendererManager::GetMeshList())
        {
            switch (mesh->GetMeshType())
            {
                case MeshTypeEnum::kPolygon:
                {
                    if (VulkanRenderer::WireframeModeFlag)
                    {
                        wireframePipeline.Draw(commandBuffer, mesh);
                    }
                    else
                    {
                        if (MeshRendererManager::GetSelectedMesh() == mesh)
                        {
                            outLinePipeline.Draw(commandBuffer, mesh);
                        }
                        pbrPipeline.Draw(commandBuffer, mesh);
                    
                    }
                    break;
                }
                case MeshTypeEnum::kLine:
                {
                    wireframePipeline.Draw(commandBuffer, mesh);
                    break;
                }
            }
        }
    }
    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }

    return commandBuffer;
}

void PBRRenderPass::Destroy()
{
    ColorTexture->Destroy();
    RenderedTexture->Destroy();
    BloomTexture->Destroy();
    RenderedBloomTexture->Destroy();
    DepthTexture->Destroy();

    pbrPipeline.Destroy();
    skyboxPipeline.Destroy();
    drawLinePipeline.Destroy();
    outLinePipeline.Destroy();
    wireframePipeline.Destroy();

    RenderPass::Destroy();
}