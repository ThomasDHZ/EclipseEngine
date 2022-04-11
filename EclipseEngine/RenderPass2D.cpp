#include "RenderPass2D.h"

RenderPass2D::RenderPass2D() : RenderPass()
{
}

RenderPass2D::~RenderPass2D()
{
}

void RenderPass2D::StartUp()
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
    
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    renderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution));
    depthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution));

    CreateRenderPass();
    CreateRendererFramebuffers();
    BuildRenderPassPipelines();
    SetUpCommandBuffers();
}

void RenderPass2D::CreateRenderPass()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    VkAttachmentDescription AlebdoAttachment = {};
    AlebdoAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    AlebdoAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    AlebdoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    AlebdoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    AlebdoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    AlebdoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    AlebdoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    AlebdoAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(AlebdoAttachment);

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
        throw std::runtime_error("Failed to create GBuffer RenderPass.");
    }
}

void RenderPass2D::CreateRendererFramebuffers()
{
    SwapChainFramebuffers.resize(VulkanRenderer::GetSwapChainImageCount());

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(renderedTexture->View);
    AttachmentList.emplace_back(depthTexture->View);

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

void RenderPass2D::BuildRenderPassPipelines()
{
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 1, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        renderer2DPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/LineRendererShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/LineRendererShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, MeshPropertiesmBufferList.size());

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderLine;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        drawLinePipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/WireFrameShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/WireFrameShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, MeshPropertiesmBufferList.size());

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderWireFrame;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        wireframePipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
}

void RenderPass2D::RebuildSwapChain()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    renderedTexture->RecreateRendererTexture(RenderPassResolution);
    depthTexture->RecreateRendererTexture(RenderPassResolution);

    RenderPass::Destroy();

    CreateRenderPass();
    CreateRendererFramebuffers();
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 1, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        renderer2DPipeline->Destroy();
        renderer2DPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/LineRendererShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/LineRendererShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, MeshPropertiesmBufferList.size());

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderLine;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        drawLinePipeline->Destroy();
        drawLinePipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/WireFrameShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/WireFrameShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, MeshPropertiesmBufferList.size());

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderWireFrame;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        wireframePipeline->Destroy();
        wireframePipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    SetUpCommandBuffers();
}

void RenderPass2D::Draw(SceneProperties sceneProperties)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 1.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

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
                    if (VulkanRenderer::WireframeModeFlag)
                    {
                        vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipeline());
                        vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipelineLayout(), 0, 1, wireframePipeline->GetDescriptorSetPtr(), 0, nullptr);
                        DrawMesh(wireframePipeline, mesh, sceneProperties);
                    }
                    else
                    {
                        vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer2DPipeline->GetShaderPipeline());
                        vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer2DPipeline->GetShaderPipelineLayout(), 0, 1, renderer2DPipeline->GetDescriptorSetPtr(), 0, nullptr);
                        DrawMesh(renderer2DPipeline, mesh, sceneProperties);
                    }
                    break;
                }
                case MeshTypeEnum::kLine:
                {
                    vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipeline());
                    vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipelineLayout(), 0, 1, drawLinePipeline->GetDescriptorSetPtr(), 0, nullptr);
                    DrawLine(drawLinePipeline, mesh, sceneProperties);
                    break;
                }
            }
        }
    }
    vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }
}

void RenderPass2D::Destroy()
{
    renderedTexture->Destroy();
    depthTexture->Destroy();

    renderer2DPipeline->Destroy();
    drawLinePipeline->Destroy();
    wireframePipeline->Destroy();

    RenderPass::Destroy();
}