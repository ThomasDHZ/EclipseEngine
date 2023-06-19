#include "GLTFRenderPass.h"

GLTFRenderPass::GLTFRenderPass()
{
}

GLTFRenderPass::~GLTFRenderPass()
{
}

void GLTFRenderPass::BuildRenderPass(std::string& renderPassFile, PBRRenderPassTextureSubmitList& textures)
{
    std::string SceneInfo;
    std::ifstream SceneFile;
    SceneFile.open(PathConsts::RenderPassPath + renderPassFile);
    if (SceneFile.is_open())
    {
        while (!SceneFile.eof())
        {
            getline(SceneFile, SceneInfo);
        }
    }
    else std::cout << "Unable to open file";
    SceneFile.close();

    std::vector<VkImageView> AttachmentList;
    std::vector<VkAttachmentDescription> AttachmentDescriptionList;

    nlohmann::json json = nlohmann::json::parse(SceneInfo);

    JsonConverter::from_json(json["textureResolution"], RenderPassResolution);
    VkFormat colorFormat = json["colorTextureFormat"];
    VkFormat depthFormat = json["depthTextureFormat"];
    VkSampleCountFlagBits sampleCount = json["multiSampleCount"];

    for (int x = 0; x < json["MultiSampledTextureList"].size(); x++)
    {
        MultiSampledColorTextureList.emplace_back(std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, colorFormat, sampleCount)));
        AttachmentList.emplace_back(MultiSampledColorTextureList[x]->View);

        VkAttachmentDescription attachmentDescription = LoadTextureAttachmentDescription(json["MultiSampledTextureList"][x]);
        attachmentDescription.format = colorFormat;
        attachmentDescription.samples = sampleCount;
        AttachmentDescriptionList.emplace_back(attachmentDescription);
    }
    for (int x = 0; x < json["ColorTextureList"].size(); x++)
    {
        ColorTextureList.emplace_back(std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, colorFormat, VK_SAMPLE_COUNT_1_BIT)));
        AttachmentList.emplace_back(ColorTextureList[x]->View);

        VkAttachmentDescription attachmentDescription = LoadTextureAttachmentDescription(json["ColorTextureList"][x]);
        attachmentDescription.format = colorFormat;
        attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        AttachmentDescriptionList.emplace_back(attachmentDescription);
    }
    for (int x = 0; x < json["OutputColorTextureList"].size(); x++)
    {
        RenderedTextureList.emplace_back(std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, colorFormat, VK_SAMPLE_COUNT_1_BIT)));
        AttachmentList.emplace_back(RenderedTextureList[x]->View);

        VkAttachmentDescription attachmentDescription = LoadTextureAttachmentDescription(json["OutputColorTextureList"][x]);
        attachmentDescription.format = colorFormat;
        attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        AttachmentDescriptionList.emplace_back(attachmentDescription);
    }
    {
        DepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, sampleCount)));
        AttachmentList.emplace_back(DepthTextureList[0]->View);

        VkAttachmentDescription attachmentDescription = LoadTextureAttachmentDescription(json["DepthTexture"]);
        attachmentDescription.format = depthFormat;
        attachmentDescription.samples = sampleCount;
        AttachmentDescriptionList.emplace_back(attachmentDescription);
    }
    //{
    //    RenderedDepthTextureList.emplace_back(std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(textureResolution, sampleCount)));
    //    AttachmentList.emplace_back(RenderedDepthTextureList[0]->View);

    //    VkAttachmentDescription attachmentDescription = LoadTextureAttachmentDescription(json["OutputDepthtexture"]);
    //    attachmentDescription.format = depthFormat;
    //    attachmentDescription.samples = sampleCount;
    //    AttachmentDescriptionList.emplace_back(attachmentDescription);
    //}
    
    std::vector<VkAttachmentReference> ColorRefsList;
    ColorRefsList.emplace_back(VkAttachmentReference{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    std::vector<VkAttachmentReference> MultiSampleReferenceList;
    MultiSampleReferenceList.emplace_back(VkAttachmentReference{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

    VkAttachmentReference DepthReference = { 2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    std::vector<VkSubpassDescription> SubpassDescriptionList{};
    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = static_cast<uint32_t>(ColorRefsList.size());
    subpassDescription.pColorAttachments = ColorRefsList.data();
    subpassDescription.pDepthStencilAttachment = &DepthReference;
    subpassDescription.pResolveAttachments = MultiSampleReferenceList.data();
    SubpassDescriptionList.emplace_back(subpassDescription);

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
    renderPassInfo.subpassCount = static_cast<uint32_t>(SubpassDescriptionList.size());
    renderPassInfo.pSubpasses = SubpassDescriptionList.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(DependencyList.size());
    renderPassInfo.pDependencies = DependencyList.data();

    if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &renderPass))
    {
        throw std::runtime_error("Failed to create Buffer RenderPass.");
    }

   CreateRendererFramebuffers(AttachmentList);
}

void GLTFRenderPass::BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures)
{
   
}

void GLTFRenderPass::Destroy()
{
    vkDestroyRenderPass(VulkanRenderer::GetDevice(), renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : RenderPassFramebuffer)
    {
        vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}

VkCommandBuffer GLTFRenderPass::Draw()
{
    return VkCommandBuffer();
}

void GLTFRenderPass::OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = CMDBuffer;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;

    VkFence fence;
    vkCreateFence(VulkanRenderer::GetDevice(), &fenceCreateInfo, nullptr, &fence);
    vkQueueSubmit(VulkanRenderer::GetGraphicsQueue(), 1, &submitInfo, fence);
    vkWaitForFences(VulkanRenderer::GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(VulkanRenderer::GetDevice(), fence, nullptr);
}

void GLTFRenderPass::SetUpCommandBuffers()
{
    CommandBuffer.resize(VulkanRenderer::GetSwapChainImageCount());
    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = VulkanRenderer::GetCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &CommandBuffer[x]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers.");
        }
    }
}

VkShaderModule GLTFRenderPass::ReadShaderFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(VulkanRenderer::GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

VkPipelineShaderStageCreateInfo GLTFRenderPass::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = shaderStages;
    vertShaderStageInfo.module = ReadShaderFile(filename);
    vertShaderStageInfo.pName = "main";

    return vertShaderStageInfo;
}

VkAttachmentDescription GLTFRenderPass::LoadTextureAttachmentDescription(nlohmann::json& json)
{
    VkAttachmentDescription RenderedColorTextureAttachment = {};
    RenderedColorTextureAttachment.loadOp = json["loadOp"];
    RenderedColorTextureAttachment.storeOp = json["storeOp"];
    RenderedColorTextureAttachment.stencilLoadOp = json["stencilLoadOp"];
    RenderedColorTextureAttachment.stencilStoreOp = json["stencilStoreOp"];
    RenderedColorTextureAttachment.initialLayout = json["initialLayout"];
    RenderedColorTextureAttachment.finalLayout = json["finalLayout"];

    return RenderedColorTextureAttachment;
}

VkSubpassDependency GLTFRenderPass::LoadSubPassDependency(nlohmann::json& json)
{
    VkSubpassDependency SubpassDependency{};
    SubpassDependency.srcSubpass = json["srcSubpass"];
    SubpassDependency.dstSubpass = json["dstSubpass"];
    SubpassDependency.srcStageMask = json["srcStageMask"];
    SubpassDependency.dstStageMask = json["dstStageMask"];
    SubpassDependency.srcAccessMask = json["srcAccessMask"];
    SubpassDependency.dstAccessMask = json["dstAccessMask"];
    SubpassDependency.dependencyFlags = json["dependencyFlags"];
    return SubpassDependency;
}

void GLTFRenderPass::CreateRendererFramebuffers(std::vector<VkImageView>& AttachmentList)
{
    RenderPassFramebuffer.resize(VulkanRenderer::GetSwapChainImageCount());

    for (size_t x = 0; x < VulkanRenderer::GetSwapChainImageCount(); x++)
    {
        VkFramebufferCreateInfo frameBufferCreateInfo = {};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.renderPass = renderPass;
        frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(AttachmentList.size());
        frameBufferCreateInfo.pAttachments = AttachmentList.data();
        frameBufferCreateInfo.width = RenderPassResolution.x;
        frameBufferCreateInfo.height = RenderPassResolution.y;
        frameBufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &frameBufferCreateInfo, nullptr, &RenderPassFramebuffer[x]))
        {
            throw std::runtime_error("Failed to create FrameBuffer.");
        }
    }
}

void GLTFRenderPass::RenderPassDesc()
{
}
