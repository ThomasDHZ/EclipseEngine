#include "RenderPass.h"
#include "MeshRendererManager.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "LineRenderer2D.h"
#include "LineRenderer3D.h"

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{
}

void RenderPass::Destroy()
{
    vkDestroyRenderPass(VulkanRenderer::GetDevice(), renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : RenderPassFramebuffer)
    {
        vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}

std::vector<std::shared_ptr<Mesh>> RenderPass::GetObjectRenderList(std::shared_ptr<GameObject> obj)
{
    std::vector<std::shared_ptr<Mesh>> MeshDrawList;
    const auto componentSubType = obj->GetComponentBySubType(ComponentSubType::kRenderedObject);
    if (componentSubType->GetComponentSubType() == ComponentSubType::kRenderedObject)
    {
        std::shared_ptr<Component> component = nullptr;
        if (component = obj->GetComponentByType(ComponentType::kSpriteRenderer))
        {
            const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
            MeshDrawList.emplace_back(spriteRenderer->GetSprite());
        }
        else if (component = obj->GetComponentByType(ComponentType::kMeshRenderer))
        {
            const auto meshRenderer = static_cast<MeshRenderer*>(component.get());
            const auto model = meshRenderer->GetModel();
            MeshDrawList = model->GetMeshList();
        }
    /*    else if (component = obj->GetComponentByType(ComponentType::kLineRenderer2D))
        {
            const auto lineRenderer = static_cast<LineRenderer2D*>(component.get());
            MeshDrawList.emplace_back(lineRenderer->GetLine());
        }*/
        //else if (component = obj->GetComponentByType(ComponentType::kLineRenderer3D))
        //{
        //    const auto lineRenderer = static_cast<LineRenderer3D*>(component.get());
        //    const auto line = lineRenderer->GetLine();
        //    MeshDrawList = line->GetMeshList();
        //}
    }

    return MeshDrawList;
}

void RenderPass::OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer)
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

void RenderPass::DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh)
{
    MeshRendererManager::DrawSkybox(CommandBuffer[VulkanRenderer::GetCMDIndex()], pipeline, mesh);
}

void RenderPass::DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, PrefilterSkyboxSettings& prefilterSkyboxSettings)
{
    MeshRendererManager::DrawSkybox(CommandBuffer[VulkanRenderer::GetCMDIndex()], pipeline, mesh, prefilterSkyboxSettings);
}

void RenderPass::DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, ConstSkyBoxView& skyboxView)
{
    MeshRendererManager::DrawSkybox(CommandBuffer[VulkanRenderer::GetCMDIndex()], pipeline, mesh, skyboxView);
}

void RenderPass::SetUpCommandBuffers()
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


VkShaderModule RenderPass::ReadShaderFile(const std::string& filename)
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

VkWriteDescriptorSetAccelerationStructureKHR RenderPass::AddAcclerationStructureBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkAccelerationStructureKHR* handle)
{
    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = {};
    AccelerationDescriptorStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    AccelerationDescriptorStructure.accelerationStructureCount = 1;
    AccelerationDescriptorStructure.pAccelerationStructures = handle;
    return AccelerationDescriptorStructure;
}

VkDescriptorImageInfo RenderPass::AddRayTraceStorageImageDescriptor(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkImageLayout ImageLayout, VkImageView& ImageView)
{
    VkDescriptorImageInfo RayTraceImageDescriptor{};
    RayTraceImageDescriptor.imageView = ImageView;
    RayTraceImageDescriptor.imageLayout = ImageLayout;
    return RayTraceImageDescriptor;
}

VkPipelineShaderStageCreateInfo RenderPass::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = shaderStages;
    vertShaderStageInfo.module = ReadShaderFile(filename);
    vertShaderStageInfo.pName = "main";

    return vertShaderStageInfo;
}

void RenderPass::CreateRendererFramebuffers(std::vector<VkImageView>& AttachmentList)
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

void RenderPass::AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    DescriptorSetBinding.AccelerationStructureDescriptor = accelerationStructure;
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

VkDescriptorImageInfo RenderPass::AddTextureDescriptor(std::shared_ptr<Texture> texture)
{
    VkDescriptorImageInfo DescriptorImage{};
    DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DescriptorImage.imageView = texture->View;
    DescriptorImage.sampler = texture->Sampler;
    return DescriptorImage;
}

VkDescriptorImageInfo RenderPass::AddTextureDescriptor(VkImageView view, VkSampler sampler)
{
    VkDescriptorImageInfo DescriptorImage{};
    DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DescriptorImage.imageView = view;
    DescriptorImage.sampler = sampler;
    return DescriptorImage;
}

void RenderPass::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = TextureImageInfo;
    DescriptorSetBinding.Count = TextureImageInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void RenderPass::AddNullDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = VK_SHADER_STAGE_ALL;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.Count = 0;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}
