#include "PrefilterRenderPass.h"
#include "PrefilterRenderPass.h"
#include "GraphicsPipeline.h"
#include "Skybox.h"


PrefilterRenderPass::PrefilterRenderPass() : RenderPass()
{
}

PrefilterRenderPass::~PrefilterRenderPass()
{
}

void PrefilterRenderPass::StartUp(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize)
{
    RenderPassResolution = glm::ivec2(cubeMapSize, cubeMapSize);
    CubeMapMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(RenderPassResolution.x, RenderPassResolution.y)))) + 1;

    DrawToCubeMap = std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(glm::ivec2(RenderPassResolution.x), VK_SAMPLE_COUNT_1_BIT));
    RenderedCubeMap = std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(glm::ivec2(RenderPassResolution.x), VK_SAMPLE_COUNT_1_BIT, CubeMapMipLevels));

    skybox = std::make_shared<Skybox>(Skybox());
    skybox->StartUp();

    BuildRenderPass();
    CreateRendererFramebuffers();
    BuildRenderPassPipelines(EnvironmentCubeMap);
    SetUpCommandBuffers();
}

void PrefilterRenderPass::BuildRenderPass()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    VkAttachmentDescription CubeMapAttachment = {};
    CubeMapAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    CubeMapAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    CubeMapAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    CubeMapAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    CubeMapAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    CubeMapAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    CubeMapAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    CubeMapAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AttachmentDescriptionList.emplace_back(CubeMapAttachment);

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


    const uint32_t viewMask = 0b00111111;
    const uint32_t correlationMask = 0b00111111;

    VkRenderPassMultiviewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
    createInfo.subpassCount = 1;
    createInfo.pViewMasks = &viewMask;
    createInfo.correlationMaskCount = 1;
    createInfo.pCorrelationMasks = &correlationMask;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(AttachmentDescriptionList.size());
    renderPassInfo.pAttachments = AttachmentDescriptionList.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(DependencyList.size());
    renderPassInfo.pDependencies = DependencyList.data();
    renderPassInfo.pNext = &createInfo;

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &renderPass))
    {
        throw std::runtime_error("failed to create GBuffer RenderPass!");
    }
}

void PrefilterRenderPass::CreateRendererFramebuffers()
{
    SwapChainFramebuffers.resize(VulkanRenderer::GetSwapChainImageCount());

    for (size_t i = 0; i < VulkanRenderer::GetSwapChainImageCount(); i++)
    {
        std::vector<VkImageView> AttachmentList;
        AttachmentList.emplace_back(DrawToCubeMap->View);

        VkFramebufferCreateInfo frameBufferCreateInfo = {};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.renderPass = renderPass;
        frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(AttachmentList.size());
        frameBufferCreateInfo.pAttachments = AttachmentList.data();
        frameBufferCreateInfo.width = RenderPassResolution.x;
        frameBufferCreateInfo.height = RenderPassResolution.y;
        frameBufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &frameBufferCreateInfo, nullptr, &SwapChainFramebuffers[i]))
        {
            throw std::runtime_error("Failed to create Gbuffer FrameBuffer.");
        }
    }
}

void PrefilterRenderPass::BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap)
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

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;

    VkDescriptorImageInfo SkyboxBufferInfo;
    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyboxBufferInfo.imageView = EnvironmentCubeMap->View;
    SkyboxBufferInfo.sampler = EnvironmentCubeMap->Sampler;

    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/PrefilterShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/PrefilterShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderPBRSkyBox;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(PrefilterSkyboxSettings);

        if (prefilterPipeline == nullptr)
        {
            prefilterPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            prefilterPipeline->Destroy();
            prefilterPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
}

void PrefilterRenderPass::RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> EnvironmentCubeMap, uint32_t cubeMapSize)
{
    firstRun = true;
    RenderPassResolution = glm::ivec2(cubeMapSize, cubeMapSize);
    DrawToCubeMap->RecreateRendererTexture(RenderPassResolution);
    RenderedCubeMap->RecreateRendererTexture(RenderPassResolution);

    prefilterPipeline->Destroy();


    RenderPass::Destroy();

    BuildRenderPass();
    CreateRendererFramebuffers();
    BuildRenderPassPipelines(EnvironmentCubeMap);
    SetUpCommandBuffers();
}

void PrefilterRenderPass::Draw()
{
    if (DrawToCubeMap->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
        RenderedCubeMap->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        DrawToCubeMap->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        RenderedCubeMap->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0);
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    RenderedCubeMap->UpdateCubeMapLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0);

    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[VulkanRenderer::GetCMDIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent.width = RenderPassResolution.x;
    renderPassInfo.renderArea.extent.height = RenderPassResolution.y;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    VkRect2D rect2D{};
    rect2D.extent.width = RenderPassResolution.x;
    rect2D.extent.height = RenderPassResolution.y;
    rect2D.offset.x = 0.0f;
    rect2D.offset.y = 0.0f;

    PrefilterSkyboxSettings prefiliter;
    for (int x = 0; x < CubeMapMipLevels; x++)
    {
        VkViewport viewport{};
        viewport.width = static_cast<float>(RenderPassResolution.x * std::pow(0.5f, x));
        viewport.height = static_cast<float>(RenderPassResolution.y * std::pow(0.5f, x));
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        prefiliter.SkyboxSize = CubeMapMipLevels;
        prefiliter.roughness = (float)RenderPassResolution.x / (float)(CubeMapMipLevels - 1);

        vkCmdSetViewport(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &viewport);
        vkCmdSetScissor(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &rect2D);
        vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, prefilterPipeline->GetShaderPipeline());
        vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_GRAPHICS, prefilterPipeline->GetShaderPipelineLayout(), 0, 1, prefilterPipeline->GetDescriptorSetPtr(), 0, nullptr);
        vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        DrawSkybox(prefilterPipeline, skybox, prefiliter);
        vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);

        DrawToCubeMap->UpdateCubeMapLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Texture::CopyCubeMap(CommandBuffer[VulkanRenderer::GetCMDIndex()], DrawToCubeMap, RenderedCubeMap, x);
        DrawToCubeMap->UpdateCubeMapLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    RenderedCubeMap->UpdateCubeMapLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0);

    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void PrefilterRenderPass::Destroy()
{
    DrawToCubeMap->Destroy();
    RenderedCubeMap->Destroy();
    prefilterPipeline->Destroy();
    RenderPass::Destroy();
}