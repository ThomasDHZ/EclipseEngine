#include "BlinnPhongRenderPass.h"


BlinnPhongRenderPass::BlinnPhongRenderPass() : BaseRenderPass()
{
}

BlinnPhongRenderPass::~BlinnPhongRenderPass()
{
}

void BlinnPhongRenderPass::StartUp()
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
   ColorAttachmentList.emplace_back(ColorAttachment);
   ColorAttachmentList.emplace_back(ColorAttachment);

    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    ColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, GraphicsDevice::GetMaxSampleCount()));
    RenderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
    BloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, GraphicsDevice::GetMaxSampleCount()));
    RenderedBloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
    DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, GraphicsDevice::GetMaxSampleCount()));

    CreateRenderPass();
    CreateRendererFramebuffers();
    blinnphongPipeline = std::make_shared<BlinnPhongPipeline>(BlinnPhongPipeline(RenderPass));
    drawLinePipeline = std::make_shared<DrawLinePipeline>(DrawLinePipeline(RenderPass, ColorAttachmentList, GraphicsDevice::GetMaxSampleCount()));
    wireframePipeline = std::make_shared<WireframePipeline>(WireframePipeline(RenderPass, ColorAttachmentList, GraphicsDevice::GetMaxSampleCount()));
    SetUpCommandBuffers();
}

void BlinnPhongRenderPass::CreateRenderPass()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    VkAttachmentDescription AlebdoAttachment = {};
    AlebdoAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    AlebdoAttachment.samples = GraphicsDevice::GetMaxSampleCount();
    AlebdoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    AlebdoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    AlebdoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    AlebdoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    AlebdoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    AlebdoAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    AttachmentDescriptionList.emplace_back(AlebdoAttachment);

    VkAttachmentDescription BloomAttachment = {};
    BloomAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    BloomAttachment.samples = GraphicsDevice::GetMaxSampleCount();
    BloomAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    BloomAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    BloomAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    BloomAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    BloomAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    BloomAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    AttachmentDescriptionList.emplace_back(BloomAttachment);

    VkAttachmentDescription MultiSampledAttachment = {};
    MultiSampledAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    MultiSampledAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    MultiSampledAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    MultiSampledAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    MultiSampledAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    MultiSampledAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    MultiSampledAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    MultiSampledAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(MultiSampledAttachment);

    VkAttachmentDescription BloomMultiSampledTexture = {};
    BloomMultiSampledTexture.format = VK_FORMAT_R8G8B8A8_UNORM;
    BloomMultiSampledTexture.samples = VK_SAMPLE_COUNT_1_BIT;
    BloomMultiSampledTexture.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    BloomMultiSampledTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    BloomMultiSampledTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    BloomMultiSampledTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    BloomMultiSampledTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    BloomMultiSampledTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(BloomMultiSampledTexture);

    VkAttachmentDescription DepthAttachment = {};
    DepthAttachment.format = VK_FORMAT_D32_SFLOAT;
    DepthAttachment.samples = GraphicsDevice::GetMaxSampleCount();
    DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    AttachmentDescriptionList.emplace_back(DepthAttachment);

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

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &RenderPass))
    {
        throw std::runtime_error("Failed to create GBuffer RenderPass.");
    }
}

void BlinnPhongRenderPass::CreateRendererFramebuffers()
{
    SwapChainFramebuffers.resize(VulkanRenderer::GetSwapChainImageCount());

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(ColorTexture->View);
    AttachmentList.emplace_back(BloomTexture->View);
    AttachmentList.emplace_back(RenderedTexture->View);
    AttachmentList.emplace_back(RenderedBloomTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(AttachmentList.size());
        framebufferInfo.pAttachments = AttachmentList.data();
        framebufferInfo.width = RenderPassResolution.x;
        framebufferInfo.height = RenderPassResolution.y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &framebufferInfo, nullptr, &SwapChainFramebuffers[x]))
        {
            throw std::runtime_error("Failed to create Gbuffer FrameBuffer.");
        }
    }
}

void BlinnPhongRenderPass::RebuildSwapChain()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    ColorTexture->RecreateRendererTexture(RenderPassResolution);
    RenderedTexture->RecreateRendererTexture(RenderPassResolution);
    BloomTexture->RecreateRendererTexture(RenderPassResolution);
    RenderedBloomTexture->RecreateRendererTexture(RenderPassResolution);
    DepthTexture->RecreateRendererTexture(RenderPassResolution);

    blinnphongPipeline->Destroy();
    drawLinePipeline->Destroy();
    BaseRenderPass::Destroy();

    CreateRenderPass();
    CreateRendererFramebuffers();
    blinnphongPipeline->UpdateGraphicsPipeLine(RenderPass);
    drawLinePipeline->UpdateGraphicsPipeLine(RenderPass, ColorAttachmentList, GraphicsDevice::GetMaxSampleCount());
    wireframePipeline->UpdateGraphicsPipeLine(RenderPass, ColorAttachmentList, GraphicsDevice::GetMaxSampleCount());
    SetUpCommandBuffers();
}

void BlinnPhongRenderPass::Draw(SceneProperties sceneProperties)
{

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 5> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[2].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[3].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[4].depthStencil = { 1.0f, 0 };

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    {
        vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, blinnphongPipeline->GetShaderPipeline());
        vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, blinnphongPipeline->GetShaderPipelineLayout(), 0, 1, blinnphongPipeline->GetDescriptorSetPtr(), 0, nullptr);

        if (VulkanRenderer::WireframeModeFlag)
        {
            //Wireframe Render Pass
            {
                vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipeline());
                vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipelineLayout(), 0, 1, wireframePipeline->GetDescriptorSetPtr(), 0, nullptr);
                for (auto obj : GameObjectManager::GetGameObjectList())
                {
                    ComponentRenderer* componentRenderer = nullptr;

                    if (obj->GetComponentByType(ComponentType::kSpriteRenderer) ||
                        obj->GetComponentByType(ComponentType::kMeshRenderer))
                    {
                        if (obj->GetComponentByType(ComponentType::kSpriteRenderer))
                        {
                            componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kSpriteRenderer).get());
                        }

                        if (obj->GetComponentByType(ComponentType::kMeshRenderer))
                        {
                            componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kMeshRenderer).get());
                        }
                    }
                    else
                    {
                        continue;
                    }

                    sceneProperties.MeshIndex = componentRenderer->GetMeshBufferIndex();
                    vkCmdPushConstants(CommandBuffer[VulkanRenderer::GetCMDIndex()], wireframePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);

                    obj->Draw(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
                }
            }
        }
        else
        {
            //Main Render Pass
            for (auto obj : GameObjectManager::GetGameObjectList())
            {
                ComponentRenderer* componentRenderer = nullptr;

                if (obj->GetComponentByType(ComponentType::kSpriteRenderer) ||
                    obj->GetComponentByType(ComponentType::kMeshRenderer))
                {
                    if (obj->GetComponentByType(ComponentType::kSpriteRenderer))
                    {
                        componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kSpriteRenderer).get());
                    }

                    if (obj->GetComponentByType(ComponentType::kMeshRenderer))
                    {
                        componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kMeshRenderer).get());
                    }
                }
                else
                {
                    continue;
                }

                sceneProperties.MeshIndex = componentRenderer->GetMeshBufferIndex();
                vkCmdPushConstants(CommandBuffer[VulkanRenderer::GetCMDIndex()], blinnphongPipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);

                obj->Draw(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
            }
        }
    }


        //Line Render Pass
        {
            vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipeline());
            vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipelineLayout(), 0, 1, drawLinePipeline->GetDescriptorSetPtr(), 0, nullptr);
            for (auto obj : GameObjectManager::GetGameObjectList())
            {
                ComponentRenderer* componentRenderer = nullptr;

                if (obj->GetComponentByType(ComponentType::kLineRenderer))
                {
                    if (obj->GetComponentByType(ComponentType::kLineRenderer))
                    {
                        componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kLineRenderer).get());
                    }
                }
                else
                {
                    continue;
                }

                sceneProperties.MeshIndex = componentRenderer->GetMeshBufferIndex();
                vkCmdPushConstants(CommandBuffer[VulkanRenderer::GetCMDIndex()], drawLinePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);

                obj->Draw(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
            }
        }
    
        vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
        if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer.");
        }
   
}

void BlinnPhongRenderPass::Destroy()
{
    ColorTexture->Destroy();
    RenderedTexture->Destroy();
    BloomTexture->Destroy();
    RenderedBloomTexture->Destroy();
    DepthTexture->Destroy();

    blinnphongPipeline->Destroy();
    drawLinePipeline->Destroy();
    wireframePipeline->Destroy();

    BaseRenderPass::Destroy();
}
