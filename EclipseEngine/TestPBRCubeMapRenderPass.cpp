#include "TestPBRCubeMapRenderPass.h"

TestPBRCubeMapRenderPass::TestPBRCubeMapRenderPass() : RenderPass()
{
}


TestPBRCubeMapRenderPass::~TestPBRCubeMapRenderPass()
{
}

void TestPBRCubeMapRenderPass::BuildRenderPass(std::string& renderPass2, PBRRenderPassTextureSubmitList& textures)
{
    SampleCount = VK_SAMPLE_COUNT_1_BIT;
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    if (renderPass == nullptr)
    {
        DrawToTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
        DrawToDepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));

        RenderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
        DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));
    }
    else
    {
        DrawToTexture->RecreateRendererTexture(RenderPassResolution);
        DrawToDepthTexture->RecreateRendererTexture(RenderPassResolution);

        RenderedTexture->RecreateRendererTexture(RenderPassResolution);
        DepthTexture->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(DrawToTexture->View);
    AttachmentList.emplace_back(DrawToDepthTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(textures);
    SetUpCommandBuffers();
}

void TestPBRCubeMapRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;
    AttachmentDescriptionList.emplace_back(DrawToTexture->GetAttachmentDescription());
    AttachmentDescriptionList.emplace_back(DrawToDepthTexture->GetAttachmentDescription());

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


void TestPBRCubeMapRenderPass::BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures)
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

    //Main Renderers
    {
        pbrReflectionPipeline = JsonGraphicsPipeline("PBRReflectionShader.txt", Vertex3D::getBindingDescriptions(), Vertex3D::getAttributeDescriptions(), renderPass, ColorAttachmentList, SampleCount, sizeof(SceneProperties), ReflectionMapSampler, textures);
        skyboxPipeline = JsonGraphicsPipeline("SkyBoxPipeline.txt", SkyboxVertexLayout::getBindingDescriptions(), SkyboxVertexLayout::getAttributeDescriptions(), renderPass, ColorAttachmentList, SampleCount, sizeof(SkyBoxView), textures.CubeMapTexture);
    }

    //Instanced Renderers
    {
    }
}

void TestPBRCubeMapRenderPass::UpdateView(glm::vec3 reflectPoint)
{
    const glm::vec3 reflectPos = reflectPoint;
    glm::mat4 reflectionProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);

    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    ReflectionMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    ReflectionMapSampler.Update();
}

VkCommandBuffer TestPBRCubeMapRenderPass::Draw(glm::vec3 reflectPoint)
{
    UpdateView(reflectPoint);

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

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &rect2D);


   // GLTFSceneManager::sceneProperites.CubeMapView = 0;

    const glm::vec3 reflectPos = reflectPoint;
    GLTFSceneManager::sceneProperites.proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);
    GLTFSceneManager::sceneProperites.view = glm::lookAt(reflectPos, reflectPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    skyboxPipeline.DrawCubeMap<SkyBoxView>(commandBuffer, GLTFSceneManager::SkyboxMesh->skyBoxView);
    for (int x = 0; x < GLTFSceneManager::GameObjectList.size(); x++)
    {
        switch (GLTFSceneManager::GameObjectList[x]->RenderType)
        {
        case GameObjectRenderType::kModelRenderer:
        {
            if (GLTFSceneManager::WireframeModeFlag)
            {
               // WireframePipeline.DrawMesh(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            else
            {
                pbrReflectionPipeline.DrawMesh(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            break;
        }
        case GameObjectRenderType::kInstanceRenderer:
        {
            if (GLTFSceneManager::WireframeModeFlag)
            {
               // WireframeInstancePipeline.DrawInstancedMesh(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            else
            {
               // PBRInstancePipeline.DrawInstancedMesh(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            break;
        }
        case GameObjectRenderType::kSpriteRenderer:
        {
            if (GLTFSceneManager::WireframeModeFlag)
            {
              //  WireframePipeline.DrawSprite(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            else
            {
                pbrReflectionPipeline.DrawSprite(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            }
            break;
        }
        case GameObjectRenderType::kDebugRenderer:
        {
           // lightReflectionPipeline.DrawMesh(commandBuffer, GLTFSceneManager::GameObjectList[x], GLTFSceneManager::sceneProperites);
            break;
        }
        case GameObjectRenderType::kLineRenderer3D:
        {
            //LinePipeline.DrawLine(commandBuffer, GLTFSceneManager::GameObjectList[x]);
            break;
        }
        }
    }

    DrawToTexture->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    //for (unsigned int x = 0; x < 6; x++)
    //{
        Texture::CopyTexture(commandBuffer, DrawToTexture, RenderedTexture);
    //}
    DrawToTexture->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    DrawToDepthTexture->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
 /*   for (unsigned int x = 0; x < 6; x++)
    {*/
        Texture::CopyDepthTexture(commandBuffer, DrawToDepthTexture, DepthTexture);
   /* }*/
    DrawToDepthTexture->UpdateCubeMapLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }

    return commandBuffer;
}

void TestPBRCubeMapRenderPass::Destroy()
{
    //ColorTexture->Destroy();
    //RenderedTexture->Destroy();
    //DepthTexture->Destroy();

    //PBRPipeline.Destroy();
    //PBRInstancePipeline.Destroy();
    //WireframePipeline.Destroy();
    //WireframeInstancePipeline.Destroy();
    //LinePipeline.Destroy();
    //SkyBoxPipeline.Destroy();

    RenderPass::Destroy();
}