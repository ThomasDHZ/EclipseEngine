#include "PBRRenderPass.h"
#include "LightManager.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"


PBRRenderPass::PBRRenderPass() : RenderPass()
{
}

PBRRenderPass::~PBRRenderPass()
{
}

void PBRRenderPass::BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceTexture, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterTexture)
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
    BuildRenderPassPipelines(reflectionIrradianceTexture, reflectionPrefilterTexture);
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

void PBRRenderPass::BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceTexture, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterTexture)
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
    ColorAttachmentList.resize(2, ColorAttachment);

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = AddAcclerationStructureBinding(DescriptorBindingList, TopLevelAccelerationStructureManager::GetAccelerationStructureHandlePtr());
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    VkDescriptorImageInfo IrradianceMapBuffer;
        IrradianceMapBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        IrradianceMapBuffer.imageView = reflectionIrradianceTexture->View;
        IrradianceMapBuffer.sampler = reflectionIrradianceTexture->Sampler;

    VkDescriptorImageInfo PrefilterBuffer;
        PrefilterBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        PrefilterBuffer.imageView = reflectionPrefilterTexture->View;
        PrefilterBuffer.sampler = reflectionPrefilterTexture->Sampler;


    VkDescriptorImageInfo BRDFBuffer;
    BRDFBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BRDFBuffer.imageView = SceneManager::BRDFTexture->View;
    BRDFBuffer.sampler = SceneManager::BRDFTexture->Sampler;

    {

        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/PBRRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/PBRRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, DirectionalLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, PointLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, SpotLightBufferInfoList);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 4, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 5, IrradianceMapBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 6, PrefilterBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 7, BRDFBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        if (pbrPipeline == nullptr)
        {
            pbrPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            pbrPipeline->Destroy();
            pbrPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
    {
        VkDescriptorImageInfo SkyboxBufferInfo;
        SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        SkyboxBufferInfo.imageView = SceneManager::CubeMap->View;
        SkyboxBufferInfo.sampler = SceneManager::CubeMap->Sampler;

        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/CubeMapVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/CubeMapFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderSkybox;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(ConstSkyBoxView);
        buildGraphicsPipelineInfo.IncludeVertexDescriptors = true;

        if (skyboxPipeline == nullptr)
        {
            skyboxPipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            skyboxPipeline->Destroy();
            skyboxPipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

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
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderLine;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);
        buildGraphicsPipelineInfo.IncludeVertexDescriptors = true;

        if (drawLinePipeline == nullptr)
        {
            drawLinePipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            drawLinePipeline->Destroy();
            drawLinePipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

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
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderWireFrame;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        if (wireframePipeline == nullptr)
        {
            wireframePipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            wireframePipeline->Destroy();
            wireframePipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }

    {
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/OutlineRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
        PipelineShaderStageList.emplace_back(CreateShader("Shaders/OutlineRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, MeshPropertiesmBufferList.size());

        BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
        buildGraphicsPipelineInfo.ColorAttachments = ColorAttachmentList;
        buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
        buildGraphicsPipelineInfo.renderPass = renderPass;
        buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
        buildGraphicsPipelineInfo.sampleCount = SampleCount;
        buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderStencil;
        buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

        if (outLinePipeline == nullptr)
        {
            outLinePipeline = std::make_shared<GraphicsPipeline>(GraphicsPipeline(buildGraphicsPipelineInfo));
        }
        else
        {
            outLinePipeline->Destroy();
            outLinePipeline->UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
        }

        for (auto& shader : PipelineShaderStageList)
        {
            vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
        }
    }
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
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline->GetShaderPipeline());
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline->GetShaderPipelineLayout(), 0, 1, skyboxPipeline->GetDescriptorSetPtr(), 0, nullptr);
        DrawSkybox(skyboxPipeline, SceneManager::GetSkyboxMesh(), SceneManager::cubeMapInfo);

        for (auto& mesh : MeshRendererManager::GetMeshList())
        {
            switch (mesh->GetMeshType())
            {
            case MeshTypeEnum::kPolygon:
            {
                if (VulkanRenderer::WireframeModeFlag)
                {
                    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipeline());
                    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline->GetShaderPipelineLayout(), 0, 1, wireframePipeline->GetDescriptorSetPtr(), 0, nullptr);
                    GameObjectManager::DrawMesh(commandBuffer, wireframePipeline, mesh, SceneManager::sceneProperites);
                }
                else
                {
                    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pbrPipeline->GetShaderPipeline());
                    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pbrPipeline->GetShaderPipelineLayout(), 0, 1, pbrPipeline->GetDescriptorSetPtr(), 0, nullptr);
                    GameObjectManager::DrawMesh(commandBuffer, pbrPipeline, mesh, SceneManager::sceneProperites);

                    if (MeshRendererManager::GetSelectedMesh() == mesh)
                    {
                        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, outLinePipeline->GetShaderPipeline());
                        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, outLinePipeline->GetShaderPipelineLayout(), 0, 1, outLinePipeline->GetDescriptorSetPtr(), 0, nullptr);
                        GameObjectManager::DrawMesh(commandBuffer, outLinePipeline, mesh, SceneManager::sceneProperites);
                    }
                }
                break;
            }
            case MeshTypeEnum::kLine:
            {
                vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipeline());
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawLinePipeline->GetShaderPipelineLayout(), 0, 1, drawLinePipeline->GetDescriptorSetPtr(), 0, nullptr);
                GameObjectManager::DrawLine(commandBuffer, drawLinePipeline, mesh, SceneManager::sceneProperites);
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

    pbrPipeline->Destroy();
    skyboxPipeline->Destroy();
    drawLinePipeline->Destroy();
    outLinePipeline->Destroy();
    wireframePipeline->Destroy();

    RenderPass::Destroy();
}