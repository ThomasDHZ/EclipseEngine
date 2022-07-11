#include "GBufferRenderPass.h"

GBufferRenderPass::GBufferRenderPass() : RenderPass()
{
}

GBufferRenderPass::~GBufferRenderPass()
{
}

void GBufferRenderPass::BuildRenderPass(std::shared_ptr<RenderedColorTexture> shadowMap)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    if (renderPass == nullptr)
    {
        PositionTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        TangentTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        BiTangentTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        TBNormalTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        NormalTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
        AlbedoTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
        SpecularTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
        BloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
        DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
    }
    else
    {
        PositionTexture->RecreateRendererTexture(RenderPassResolution);
        TangentTexture->RecreateRendererTexture(RenderPassResolution);
        BiTangentTexture->RecreateRendererTexture(RenderPassResolution);
        TBNormalTexture->RecreateRendererTexture(RenderPassResolution);
        NormalTexture->RecreateRendererTexture(RenderPassResolution);
        AlbedoTexture->RecreateRendererTexture(RenderPassResolution);
        SpecularTexture->RecreateRendererTexture(RenderPassResolution);
        BloomTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(PositionTexture->View);
    AttachmentList.emplace_back(TangentTexture->View);
    AttachmentList.emplace_back(BiTangentTexture->View);
    AttachmentList.emplace_back(TBNormalTexture->View);
    AttachmentList.emplace_back(NormalTexture->View);
    AttachmentList.emplace_back(AlbedoTexture->View);
    AttachmentList.emplace_back(SpecularTexture->View);
    AttachmentList.emplace_back(BloomTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(shadowMap);
    SetUpCommandBuffers();
}

void GBufferRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(PositionTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(TangentTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(BiTangentTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(TBNormalTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(NormalTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(AlbedoTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(SpecularTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(BloomTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(DepthTexture->GetAttachmentDescription());

    std::vector<VkAttachmentReference> ColorRefsList;
    ColorRefsList.emplace_back(VkAttachmentReference{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 6, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 7, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkAttachmentReference depthReference = { 8, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

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

void GBufferRenderPass::BuildRenderPassPipelines(std::shared_ptr<RenderedColorTexture> shadowMap)
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
    ColorAttachmentList.resize(8, ColorAttachment);

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;

    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = AddAcclerationStructureBinding(DescriptorBindingList, TopLevelAccelerationStructureManager::GetAccelerationStructureHandlePtr());
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
    {
        VkDescriptorImageInfo ShadowMapBufferInfo;
        ShadowMapBufferInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ShadowMapBufferInfo.imageView = shadowMap->View;
        ShadowMapBufferInfo.sampler = shadowMap->Sampler;

        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/GBufferVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/GBufferFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, DirectionalLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, PointLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, SpotLightBufferInfoList);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 4, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
        AddAccelerationDescriptorSetBinding(DescriptorBindingList, 5, AccelerationDescriptorStructure, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);
        buildGraphicsPipelineInfo.IncludeVertexDescriptors = true;

        if (hybridPipeline == nullptr)
        {
            hybridPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            hybridPipeline->Destroy();
            hybridPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
}

VkCommandBuffer GBufferRenderPass::Draw()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 9> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[2].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[3].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[4].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[5].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[6].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[7].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[8].depthStencil = { 1.0f, 0 };

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
        for (auto& mesh : MeshRendererManager::GetMeshList())
        {
            switch (mesh->GetMeshType())
            {
                vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, hybridPipeline->GetShaderPipeline());
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, hybridPipeline->GetShaderPipelineLayout(), 0, 1, hybridPipeline->GetDescriptorSetPtr(), 0, nullptr);
                GameObjectManager::DrawMesh(commandBuffer, hybridPipeline, mesh, SceneManager::sceneProperites);
            }
        }
    }
    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }

    return commandBuffer;
}

void GBufferRenderPass::Destroy()
{
    PositionTexture->Destroy();
    TangentTexture->Destroy();
    BiTangentTexture->Destroy();
    TBNormalTexture->Destroy();
    NormalTexture->Destroy();
    AlbedoTexture->Destroy();
    SpecularTexture->Destroy();
    BloomTexture->Destroy();
    DepthTexture->Destroy();

    hybridPipeline->Destroy();

    RenderPass::Destroy();
}
