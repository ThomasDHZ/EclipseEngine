#include "CubeToEnvironmentRenderPass.h"

#include <tinygltf/stb_image_write.h>

CubeToEnvironmentRenderPass::CubeToEnvironmentRenderPass() : RenderPass()
{
}

CubeToEnvironmentRenderPass::~CubeToEnvironmentRenderPass()
{
}

void CubeToEnvironmentRenderPass::BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
    }
    else
    {
        ImageTexture->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(ImageTexture->View);

    GLTFSceneManager::LoadTexture2D(ImageTexture);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(cubeMap);
    SetUpCommandBuffers();
}

void CubeToEnvironmentRenderPass::OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R32G32B32A32_SFLOAT, VK_SAMPLE_COUNT_1_BIT));
    }
    else
    {
        ImageTexture->RecreateRendererTexture(RenderPassResolution);
        RenderPass::Destroy();
    }

    std::vector<VkImageView> AttachmentList;
    AttachmentList.emplace_back(ImageTexture->View);

    RenderPassDesc();
    CreateRendererFramebuffers(AttachmentList);
    BuildRenderPassPipelines(cubeMap);
    SetUpCommandBuffers();
    Draw(0.0f);
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void CubeToEnvironmentRenderPass::RenderPassDesc()
{
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    VkAttachmentDescription CubeMapAttachment = {};
    AttachmentDescriptionList.emplace_back(ImageTexture->GetAttachmentDescription());

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
        throw std::runtime_error("failed to create BRDF RenderPass!");
    }
}

void CubeToEnvironmentRenderPass::BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMap)
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

    ImagePipeline = JsonGraphicsPipeline("/Pipelines/EnvironmentToCubeMapPipeline.txt", VoidVertex::getBindingDescriptions(), VoidVertex::getAttributeDescriptions(), renderPass, ColorAttachmentList, SampleCount, sizeof(TextureCreatorProperties), cubeMap);
}

VkCommandBuffer CubeToEnvironmentRenderPass::Draw(float time)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = RenderPassFramebuffer[VulkanRenderer::GetImageIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = { (uint32_t)RenderPassResolution.x, (uint32_t)RenderPassResolution.y };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

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

    textureCreatorProperties.time = time;

    if (vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
    }
    vkCmdSetViewport(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &viewport);
    vkCmdSetScissor(CommandBuffer[VulkanRenderer::GetCMDIndex()], 0, 1, &rect2D);
    vkCmdBeginRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    ImagePipeline.DrawTexture(CommandBuffer[VulkanRenderer::GetCMDIndex()], textureCreatorProperties);
    vkCmdEndRenderPass(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
    if (vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
    }

    return CommandBuffer[VulkanRenderer::GetCMDIndex()];
}

void CubeToEnvironmentRenderPass::Destroy()
{
    ImageTexture->Destroy();
    ImagePipeline.Destroy();
    RenderPass::Destroy();
}

void CubeToEnvironmentRenderPass::SaveTexture(const char* filename)
{
    std::shared_ptr<BakedTexture> BakeTexture = std::make_shared<BakedTexture>(BakedTexture(Pixel32(0.0f, 0.0f, 0.0f, 1.0f), glm::vec2(ImageTexture->GetWidth(), ImageTexture->GetHeight()), VK_FORMAT_R32G32B32A32_SFLOAT));

    VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

    BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    ImageTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    VkImageCopy copyImage{};
    copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyImage.srcSubresource.layerCount = 1;

    copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyImage.dstSubresource.layerCount = 1;

    copyImage.extent.width = ImageTexture->GetWidth();
    copyImage.extent.height = ImageTexture->GetHeight();
    copyImage.extent.depth = 1;

    vkCmdCopyImage(commandBuffer, ImageTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, BakeTexture->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);

    BakeTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL);
    ImageTexture->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    VulkanRenderer::EndSingleTimeCommands(commandBuffer);

    VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
    VkSubresourceLayout subResourceLayout;
    vkGetImageSubresourceLayout(VulkanRenderer::GetDevice(), BakeTexture->Image, &subResource, &subResourceLayout);
 
    const float* data;
    vkMapMemory(VulkanRenderer::GetDevice(), BakeTexture->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);


    stbi_write_hdr(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data);

    BakeTexture->Destroy();
}