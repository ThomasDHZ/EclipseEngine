#include "PBRReflectionPreRenderPass.h"
#include "LightManager.h"


PBRReflectionPreRenderPass::PBRReflectionPreRenderPass() : RenderPass()
{
}

PBRReflectionPreRenderPass::~PBRReflectionPreRenderPass()
{
}

void PBRReflectionPreRenderPass::BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = glm::vec2(cubeMapSize);

    if (renderPass == nullptr)
    {
        RenderedTexture = std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        DepthTexture = std::make_shared<RenderedCubeMapDepthTexture>(RenderedCubeMapDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
    }
    else
    {
        ClearTextureList();
        RenderedTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(RenderedTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
}

void PBRReflectionPreRenderPass::PreRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = glm::vec2(cubeMapSize);

    if (renderPass == nullptr)
    {
        RenderedTexture = std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        DepthTexture = std::make_shared<RenderedCubeMapDepthTexture>(RenderedCubeMapDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
    }
    else
    {
        ClearTextureList();
        RenderedTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(RenderedTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
    Draw();
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void PBRReflectionPreRenderPass::BakeReflectionMaps(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, uint32_t bakedTextureAtlusSize)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = glm::vec2(cubeMapSize);

    if (renderPass == nullptr)
    {
        RenderedTexture = std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        DepthTexture = std::make_shared<RenderedCubeMapDepthTexture>(RenderedCubeMapDepthTexture(RenderPassResolution, VK_SAMPLE_COUNT_1_BIT));
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
    }
    else
    {
        ClearTextureList();
        RenderedTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        for (const auto& mesh : MeshRendererManager::GetMeshList())
        {
            ReflectionCubeMapList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(RenderPassResolution, SampleCount)));
        }
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(RenderedTexture->View);
    AttachmentList.emplace_back(DepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
    Draw();
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    TextureBaker::BakeCubeTextureAtlus("../texture/TestReflectionBakeLayer", ReflectionCubeMapList, cubeMapSize, bakedTextureAtlusSize);
}


void PBRReflectionPreRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(RenderedTexture->GetAttachmentDescription());

    VkAttachmentDescription DepthAttachment = {};
    DepthAttachment.format = VK_FORMAT_D32_SFLOAT;
    DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
        throw std::runtime_error("Failed to create Buffer RenderPass.");
    }

}

void PBRReflectionPreRenderPass::BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures)
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

    pbrPipeline.InitializePipeline(pipelineInfo, textures);
    skyboxPipeline.InitializePipeline(pipelineInfo, SceneManager::CubeMap);
}

void PBRReflectionPreRenderPass::ClearTextureList()
{
    for (auto& reflectionTexture : ReflectionCubeMapList)
    {
        reflectionTexture->Destroy();
    }
    ReflectionCubeMapList.clear();
}

VkCommandBuffer PBRReflectionPreRenderPass::Draw()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
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
    renderPassInfo.framebuffer = RenderPassFramebuffer[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = { (uint32_t)RenderPassResolution.x, (uint32_t)RenderPassResolution.y };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    VkCommandBuffer commandBuffer = CommandBuffer[VulkanRenderer::GetCMDIndex()];
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);
    for (int x = 0; x < MeshRendererManager::GetMeshList().size(); x++)
    {
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        const std::shared_ptr<Mesh> reflectingMesh = MeshRendererManager::GetMeshList()[x];
        skyboxPipeline.Draw(commandBuffer);
        for (auto& mesh : MeshRendererManager::GetMeshList())
        {

            switch (mesh->GetMeshType())
            {

            case MeshTypeEnum::kPolygon:
            {
                pbrPipeline.Draw(commandBuffer, mesh, reflectingMesh, x);
                break;
            }
            default: break;
            }
        }
        vkCmdEndRenderPass(commandBuffer);

        RenderedTexture->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        ReflectionCubeMapList[x]->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Texture::CopyCubeMap(commandBuffer, RenderedTexture, ReflectionCubeMapList[x]);
        ReflectionCubeMapList[x]->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        RenderedTexture->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }
   
    return commandBuffer;
}

void PBRReflectionPreRenderPass::Destroy()
{
    for (auto reflectionMap : ReflectionCubeMapList)
    {
        reflectionMap->Destroy();
    }

    RenderedTexture->Destroy();
    DepthTexture->Destroy();

    pbrPipeline.Destroy();
    skyboxPipeline.Destroy();

    RenderPass::Destroy();
}