#include "GLTFRenderPass.h"

GLTFRenderPass::GLTFRenderPass()
{
}

GLTFRenderPass::~GLTFRenderPass()
{
}

void GLTFRenderPass::BuildRenderPass(PBRRenderPassTextureSubmitList& textures)
{
    //SampleCount = GraphicsDevice::GetMaxSampleCount();
    //RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    //if (renderPass == nullptr)
    //{
    //    MultiSampledColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
    //    ColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    //    DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));

    //    RenderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    //}
    //else
    //{
    //    MultiSampledColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, SampleCount));
    //    ColorTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    //    DepthTexture = std::make_shared<RenderedDepthTexture>(RenderedDepthTexture(RenderPassResolution, SampleCount));

    //    RenderedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT));
    //    GLTFRenderPass::Destroy();
    //}

    //std::vector<VkImageView> AttachmentList;
    //AttachmentList.emplace_back(ColorTexture->View);
    //AttachmentList.emplace_back(RenderedTexture->View);
    //AttachmentList.emplace_back(DepthTexture->View);
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
