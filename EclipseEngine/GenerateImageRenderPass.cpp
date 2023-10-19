#include "GenerateImageRenderPass.h"
#include <tinygltf/stb_image_write.h>

GenerateImageRenderPass::GenerateImageRenderPass() : RenderPass()
{
}

GenerateImageRenderPass::~GenerateImageRenderPass()
{
}

void GenerateImageRenderPass::BuildRenderPass(const char* pipelineFileName, VkFormat textureFormat, glm::ivec2 textureSize)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, textureFormat, VK_SAMPLE_COUNT_1_BIT));
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
    BuildRenderPassPipelines(pipelineFileName);
    SetUpCommandBuffers();
}

void GenerateImageRenderPass::BuildRenderPass(const char* pipelineFileName, glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
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
    BuildRenderPassPipelines(pipelineFileName, texture1, texture2);
    SetUpCommandBuffers();
}

void GenerateImageRenderPass::OneTimeDraw(const char* pipelineFileName, VkFormat textureFormat, glm::ivec2 textureSize)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, textureFormat, VK_SAMPLE_COUNT_1_BIT));
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
    BuildRenderPassPipelines(pipelineFileName);
    SetUpCommandBuffers();
    Draw(0.0f);
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void GenerateImageRenderPass::OneTimeDraw(const char* pipelineFileName, glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2)
{
    RenderPassResolution = textureSize;

    if (renderPass == nullptr)
    {
        ImageTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
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
    BuildRenderPassPipelines(pipelineFileName, texture1, texture2);
    SetUpCommandBuffers();
    Draw(0.0f);
    OneTimeRenderPassSubmit(&CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void GenerateImageRenderPass::RenderPassDesc()
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

void GenerateImageRenderPass::BuildRenderPassPipelines(const char* pipelineFileName)
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

    ImagePipeline = JsonGraphicsPipeline(pipelineFileName, VoidVertex::getBindingDescriptions(), VoidVertex::getAttributeDescriptions(), renderPass, ColorAttachmentList, SampleCount, sizeof(TextureCreatorProperties));
}

void GenerateImageRenderPass::BuildRenderPassPipelines(const char* pipelineFileName, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2)
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

    ImagePipeline = JsonGraphicsPipeline(pipelineFileName, VoidVertex::getBindingDescriptions(), VoidVertex::getAttributeDescriptions(), renderPass, ColorAttachmentList, SampleCount, sizeof(TextureCreatorProperties), texture1, texture2);
}

VkCommandBuffer GenerateImageRenderPass::Draw(float time)
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

void GenerateImageRenderPass::Destroy()
{
    ImageTexture->Destroy();
    ImagePipeline.Destroy();
    RenderPass::Destroy();
}

void GenerateImageRenderPass::SaveTexture(const char* filename, BakeTextureFormat textureFormat)
{
    std::shared_ptr<BakedTexture> BakeTexture = std::make_shared<BakedTexture>(BakedTexture(Pixel(0x00, 0x00, 0x00, 0xFF), glm::vec2(ImageTexture->GetWidth(), ImageTexture->GetHeight())));

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

	const char* data;
	vkMapMemory(VulkanRenderer::GetDevice(), BakeTexture->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);

	switch (textureFormat)
	{
	case BakeTextureFormat::Bake_BMP: stbi_write_bmp(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data); break;
	case BakeTextureFormat::Bake_JPG: stbi_write_jpg(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data, 100); break;
	case BakeTextureFormat::Bake_PNG: stbi_write_png(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data, STBI_rgb_alpha * ImageTexture->GetWidth()); break;
	case BakeTextureFormat::Bake_TGA: stbi_write_tga(filename, BakeTexture->GetWidth(), BakeTexture->GetHeight(), STBI_rgb_alpha, data); break;
	}

	BakeTexture->Destroy();
}
