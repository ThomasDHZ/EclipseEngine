#include "GBufferRenderPass.h"

GBufferRenderPass::GBufferRenderPass() : RenderPass()
{
}

GBufferRenderPass::~GBufferRenderPass()
{
}

void GBufferRenderPass::StartUp(std::shared_ptr<RenderedColorTexture> shadowMap)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    PositionTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
    NormalTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
    AlbedoTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    SpecularTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    BloomTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));

    BuildRenderPass();
    CreateRendererFramebuffers();
    BuildRenderPassPipelines(shadowMap);
    SetUpCommandBuffers();
}

void GBufferRenderPass::BuildRenderPass()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    VkAttachmentDescription PositionTexture = {};
    PositionTexture.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    PositionTexture.samples = SampleCount;
    PositionTexture.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    PositionTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    PositionTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    PositionTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    PositionTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    PositionTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(PositionTexture);

    VkAttachmentDescription NormalTexture = {};
    NormalTexture.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    NormalTexture.samples = SampleCount;
    NormalTexture.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    NormalTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    NormalTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    NormalTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    NormalTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    NormalTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(NormalTexture);

    VkAttachmentDescription AlbedoTexture = {};
    AlbedoTexture.format = VK_FORMAT_R8G8B8A8_UNORM;
    AlbedoTexture.samples = VK_SAMPLE_COUNT_1_BIT;
    AlbedoTexture.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    AlbedoTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    AlbedoTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    AlbedoTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    AlbedoTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    AlbedoTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(AlbedoTexture);

    VkAttachmentDescription SpecularTexture = {};
    SpecularTexture.format = VK_FORMAT_R8G8B8A8_UNORM;
    SpecularTexture.samples = VK_SAMPLE_COUNT_1_BIT;
    SpecularTexture.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    SpecularTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    SpecularTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    SpecularTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    SpecularTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    SpecularTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(SpecularTexture);

    VkAttachmentDescription BloomTexture = {};
    BloomTexture.format = VK_FORMAT_R8G8B8A8_UNORM;
    BloomTexture.samples = VK_SAMPLE_COUNT_1_BIT;
    BloomTexture.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    BloomTexture.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    BloomTexture.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    BloomTexture.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    BloomTexture.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    BloomTexture.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(BloomTexture);

    VkAttachmentDescription DepthAttachment = {};
    DepthAttachment.format = VK_FORMAT_D32_SFLOAT;
    DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
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
    ColorRefsList.emplace_back(VkAttachmentReference{ 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    ColorRefsList.emplace_back(VkAttachmentReference{ 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkAttachmentReference depthReference = { 5, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

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

void GBufferRenderPass::CreateRendererFramebuffers()
{
    SwapChainFramebuffers.resize(VulkanRenderer::GetSwapChainImageCount());

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(PositionTexture->View);
    AttachmentList.emplace_back(NormalTexture->View);
    AttachmentList.emplace_back(AlbedoTexture->View);
    AttachmentList.emplace_back(SpecularTexture->View);
    AttachmentList.emplace_back(BloomTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
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
    ColorAttachmentList.resize(5, ColorAttachment);

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;

    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = AddAcclerationStructureBinding(DescriptorBindingList, ModelManager::GetAccelerationStructureHandlePtr());
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

void GBufferRenderPass::RebuildSwapChain(std::shared_ptr<RenderedColorTexture> shadowMap)
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    PositionTexture->RecreateRendererTexture(RenderPassResolution);
    NormalTexture->RecreateRendererTexture(RenderPassResolution);
    AlbedoTexture->RecreateRendererTexture(RenderPassResolution);
    SpecularTexture->RecreateRendererTexture(RenderPassResolution);
    BloomTexture->RecreateRendererTexture(RenderPassResolution);
    DepthTexture->RecreateRendererTexture(RenderPassResolution);

    RenderPass::Destroy();

    BuildRenderPass();
    CreateRendererFramebuffers();
    BuildRenderPassPipelines(shadowMap);
    SetUpCommandBuffers();
}

void GBufferRenderPass::Draw(SceneProperties& sceneProperties)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 6> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[2].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[3].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[4].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[5].depthStencil = { 1.0f, 0 };

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
    renderPassInfo.framebuffer = SwapChainFramebuffers[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }

    vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &viewport);
    vkCmdSetScissor(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &rect2D);
    {
        MeshRendererManager::SortByRenderPipeline();
        for (auto& mesh : MeshRendererManager::GetMeshList())
        {
            switch (mesh->GetMeshType())
            {
                case MeshTypeEnum::kPolygon:
                {
                        vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, hybridPipeline->GetShaderPipeline());
                        vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, hybridPipeline->GetShaderPipelineLayout(), 0, 1, hybridPipeline->GetDescriptorSetPtr(), 0, nullptr);
                        DrawMesh(hybridPipeline, mesh, sceneProperties);
                }
            }
        }
    }
    vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }
}

void GBufferRenderPass::Destroy()
{
    PositionTexture->Destroy();
    NormalTexture->Destroy();
    AlbedoTexture->Destroy();
    SpecularTexture->Destroy();
    BloomTexture->Destroy();
    DepthTexture->Destroy();

    hybridPipeline->Destroy();\

    RenderPass::Destroy();
}
