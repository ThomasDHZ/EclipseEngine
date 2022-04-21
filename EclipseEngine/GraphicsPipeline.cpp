#include "GraphicsPipeline.h"
#include <fstream>

GraphicsPipeline::GraphicsPipeline()
{
}

GraphicsPipeline::GraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
{
    VkSamplerCreateInfo NullSamplerInfo = {};
    NullSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    NullSamplerInfo.magFilter = VK_FILTER_NEAREST;
    NullSamplerInfo.minFilter = VK_FILTER_NEAREST;
    NullSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    NullSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    NullSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    NullSamplerInfo.anisotropyEnable = VK_TRUE;
    NullSamplerInfo.maxAnisotropy = 16.0f;
    NullSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    NullSamplerInfo.unnormalizedCoordinates = VK_FALSE;
    NullSamplerInfo.compareEnable = VK_FALSE;
    NullSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    NullSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    NullSamplerInfo.minLod = 0;
    NullSamplerInfo.maxLod = 0;
    NullSamplerInfo.mipLodBias = 0;
    if (vkCreateSampler(VulkanRenderer::GetDevice(), &NullSamplerInfo, nullptr, &NullSampler))
    {
        throw std::runtime_error("Failed to create Sampler.");
    }

    nullBufferInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    nullBufferInfo.imageView = VK_NULL_HANDLE;
    nullBufferInfo.sampler = NullSampler;

    DescriptorPoolList.clear();
    LayoutBindingInfo.clear();
    DescriptorList.clear();

    BuildDescriptorBindings(buildGraphicsPipelineInfo);
    BuildShaderPipeLine(buildGraphicsPipelineInfo);
}

GraphicsPipeline::~GraphicsPipeline()
{
}

void GraphicsPipeline::SubmitDescriptorSet(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
    if (DescriptorBindingList.size() > 0)
    {
        {
            std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
            for (auto& DescriptorBinding : DescriptorBindingList)
            {
                DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(DescriptorBinding.DescriptorType, DescriptorBinding.Count));
            }
            DescriptorPool = CreateDescriptorPool(DescriptorPoolList);
        }
        {
            std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo = {};
            for (auto& DescriptorBinding : DescriptorBindingList)
            {
                LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.DescriptorType, DescriptorBinding.StageFlags, DescriptorBinding.Count });
            }
            DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);
        }
        {
            DescriptorSet = CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

            std::vector<VkWriteDescriptorSet> DescriptorList;

            for (auto& DescriptorBinding : DescriptorBindingList)
            {


                if (DescriptorBinding.BufferDescriptor.size() > 0)
                {
                    DescriptorList.emplace_back(AddBufferDescriptorSet(DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
                }
                else if (DescriptorBinding.TextureDescriptor.size() > 0)
                {
                    DescriptorList.emplace_back(AddTextureDescriptorSet(DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
                }
                else
                {
                    DescriptorList.emplace_back(AddAccelerationBuffer(DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
                }
            }
            vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(DescriptorList.size()), DescriptorList.data(), 0, nullptr);
        }
    }
    else
    {
        DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
        DescriptorPool = CreateDescriptorPool(DescriptorPoolList);

        LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL, 1 });
        DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);

        DescriptorSet = CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);
        vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(DescriptorList.size()), DescriptorList.data(), 0, nullptr);
    }
}

void GraphicsPipeline::UpdateGraphicsPipeLine()
{
    DescriptorPoolList.clear();
    LayoutBindingInfo.clear();
    DescriptorList.clear();

    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
    vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VulkanRenderer::GetDevice(), DescriptorSetLayout, nullptr);
    vkDestroyPipelineCache(VulkanRenderer::GetDevice(), PipelineCache, nullptr);

    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;
    DescriptorPool = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;
    PipelineCache = VK_NULL_HANDLE;
}

void GraphicsPipeline::UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
{
    DescriptorPoolList.clear();
    LayoutBindingInfo.clear();
    DescriptorList.clear();

    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
    vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VulkanRenderer::GetDevice(), DescriptorSetLayout, nullptr);
    vkDestroyPipelineCache(VulkanRenderer::GetDevice(), PipelineCache, nullptr);

    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;
    DescriptorPool = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;
    PipelineCache = VK_NULL_HANDLE;

    BuildDescriptorBindings(buildGraphicsPipelineInfo);
    BuildShaderPipeLine(buildGraphicsPipelineInfo);
}

void GraphicsPipeline::Destroy()
{
    vkDestroySampler(VulkanRenderer::GetDevice(), NullSampler, nullptr);
    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
    vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VulkanRenderer::GetDevice(), DescriptorSetLayout, nullptr);
    vkDestroyPipelineCache(VulkanRenderer::GetDevice(), PipelineCache, nullptr);

    NullSampler = VK_NULL_HANDLE;
    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;
    DescriptorPool = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;
    PipelineCache = VK_NULL_HANDLE;
}

VkWriteDescriptorSet GraphicsPipeline::AddAccelerationBuffer(uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure)
{
    VkWriteDescriptorSet AccelerationDesciptorSet = {};
    AccelerationDesciptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    AccelerationDesciptorSet.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    AccelerationDesciptorSet.dstSet = DescriptorSet;
    AccelerationDesciptorSet.dstBinding = BindingNumber;
    AccelerationDesciptorSet.descriptorCount = 1;
    AccelerationDesciptorSet.pNext = &accelerationStructure;
    return AccelerationDesciptorSet;
}

VkWriteDescriptorSet GraphicsPipeline::AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = 0;
    TextureDescriptor.descriptorType = descriptorType;
    TextureDescriptor.descriptorCount = 1;
    TextureDescriptor.pImageInfo = &TextureImageInfo;
    return TextureDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddTextureDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = 0;
    TextureDescriptor.descriptorType = descriptorType;
    TextureDescriptor.descriptorCount = static_cast<uint32_t>(TextureImageInfo.size());
    TextureDescriptor.pImageInfo = TextureImageInfo.data();
    return TextureDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = 0;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = 1;
    BufferDescriptor.pBufferInfo = &BufferInfo;
    return BufferDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddBufferDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = 0;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = static_cast<uint32_t>(BufferInfoList.size());
    BufferDescriptor.pBufferInfo = BufferInfoList.data();
    return BufferDescriptor;
}

VkDescriptorPoolSize GraphicsPipeline::AddDsecriptorPoolBinding(VkDescriptorType descriptorType, uint32_t descriptorCount)
{
    VkDescriptorPoolSize DescriptorPoolBinding = {};
    DescriptorPoolBinding.type = descriptorType;
    DescriptorPoolBinding.descriptorCount = descriptorCount;

    return DescriptorPoolBinding;
}

VkDescriptorPool GraphicsPipeline::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo)
{
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
    poolInfo.pPoolSizes = DescriptorPoolInfo.data();
    poolInfo.maxSets = static_cast<uint32_t>(VulkanRenderer::GetSwapChainImageCount());

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    return descriptorPool;
}

VkDescriptorSetLayout GraphicsPipeline::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo)
{
    std::vector<VkDescriptorSetLayoutBinding> LayoutBindingList = {};

    for (auto Binding : LayoutBindingInfo)
    {
        VkDescriptorSetLayoutBinding LayoutBinding = {};
        LayoutBinding.binding = Binding.Binding;
        LayoutBinding.descriptorCount = Binding.Count;
        LayoutBinding.descriptorType = Binding.DescriptorType;
        LayoutBinding.pImmutableSamplers = nullptr;
        LayoutBinding.stageFlags = Binding.StageFlags;

        LayoutBindingList.emplace_back(LayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindingList.size());
    layoutInfo.pBindings = LayoutBindingList.data();

    VkDescriptorSetLayout descriptorSet;
    if (vkCreateDescriptorSetLayout(VulkanRenderer::GetDevice(), &layoutInfo, nullptr, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    return descriptorSet;
}

VkDescriptorSetLayout GraphicsPipeline::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo, VkDescriptorSetLayoutBindingFlagsCreateInfoEXT& DescriptorSetLayoutBindingFlags)
{
    std::vector<VkDescriptorSetLayoutBinding> LayoutBindingList = {};

    for (auto Binding : LayoutBindingInfo)
    {
        VkDescriptorSetLayoutBinding LayoutBinding = {};
        LayoutBinding.binding = Binding.Binding;
        LayoutBinding.descriptorCount = Binding.Count;
        LayoutBinding.descriptorType = Binding.DescriptorType;
        LayoutBinding.pImmutableSamplers = nullptr;
        LayoutBinding.stageFlags = Binding.StageFlags;

        LayoutBindingList.emplace_back(LayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindingList.size());
    layoutInfo.pBindings = LayoutBindingList.data();
    layoutInfo.pNext = &DescriptorSetLayoutBindingFlags;

    VkDescriptorSetLayout descriptorSet;
    if (vkCreateDescriptorSetLayout(VulkanRenderer::GetDevice(), &layoutInfo, nullptr, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    return descriptorSet;
}

VkDescriptorSet GraphicsPipeline::CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{

    uint32_t variableDescCounts[] = { 1 };

    VkDescriptorSetVariableDescriptorCountAllocateInfoEXT VariableDescriptorCountAllocateInfo{};
    VariableDescriptorCountAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
    VariableDescriptorCountAllocateInfo.descriptorSetCount = 1;
    VariableDescriptorCountAllocateInfo.pDescriptorCounts = variableDescCounts;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;
    allocInfo.pNext = &VariableDescriptorCountAllocateInfo;

    VkDescriptorSet DescriptorSets;
    if (vkAllocateDescriptorSets(VulkanRenderer::GetDevice(), &allocInfo, &DescriptorSets) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets.");
    }

    return DescriptorSets;
}

VkDescriptorBufferInfo GraphicsPipeline::AddBufferDescriptor(VulkanBuffer& buffer)
{
    VkDescriptorBufferInfo BufferInfo = {};
    BufferInfo.buffer = buffer.GetBuffer();
    BufferInfo.offset = 0;
    BufferInfo.range = buffer.GetBufferSize();
    return BufferInfo;
}

VkWriteDescriptorSet GraphicsPipeline::AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = 0;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = 1;
    BufferDescriptor.pBufferInfo = &BufferInfo;
    return BufferDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = 0;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = static_cast<uint32_t>(BufferInfoList.size());
    BufferDescriptor.pBufferInfo = BufferInfoList.data();
    return BufferDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorImageInfo& TextureImageInfo)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = 0;
    TextureDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    TextureDescriptor.descriptorCount = 1;
    TextureDescriptor.pImageInfo = &TextureImageInfo;
    return TextureDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, std::vector<VkDescriptorImageInfo>& TextureImageInfo)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = 0;
    TextureDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    TextureDescriptor.descriptorCount = static_cast<uint32_t>(TextureImageInfo.size());
    TextureDescriptor.pImageInfo = TextureImageInfo.data();
    return TextureDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddStorageImageBuffer(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorImageInfo& TextureImageInfo)
{
    VkWriteDescriptorSet ImageDescriptor{};
    ImageDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    ImageDescriptor.dstSet = DescriptorSet;
    ImageDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    ImageDescriptor.dstBinding = BindingNumber;
    ImageDescriptor.pImageInfo = &TextureImageInfo;
    ImageDescriptor.descriptorCount = 1;
    return ImageDescriptor;
}

VkWriteDescriptorSet GraphicsPipeline::AddAccelerationBuffer(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure)
{
    VkWriteDescriptorSet AccelerationDesciptorSet = {};
    AccelerationDesciptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    AccelerationDesciptorSet.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    AccelerationDesciptorSet.dstSet = DescriptorSet;
    AccelerationDesciptorSet.dstBinding = BindingNumber;
    AccelerationDesciptorSet.descriptorCount = 1;
    AccelerationDesciptorSet.pNext = &accelerationStructure;
    return AccelerationDesciptorSet;
}

VkWriteDescriptorSetAccelerationStructureKHR GraphicsPipeline::AddAcclerationStructureBinding(VkAccelerationStructureKHR& handle)
{
    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = {};
    AccelerationDescriptorStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    AccelerationDescriptorStructure.accelerationStructureCount = 1;
    AccelerationDescriptorStructure.pAccelerationStructures = &handle;
    return AccelerationDescriptorStructure;
}

VkDescriptorImageInfo GraphicsPipeline::AddRayTraceReturnImageDescriptor(VkImageLayout ImageLayout, VkImageView& ImageView)
{
    VkDescriptorImageInfo RayTraceImageDescriptor{};
    RayTraceImageDescriptor.imageView = ImageView;
    RayTraceImageDescriptor.imageLayout = ImageLayout;
    return RayTraceImageDescriptor;
}

VkDescriptorImageInfo GraphicsPipeline::AddTextureDescriptor(VkImageView view, VkSampler sampler)
{
    VkDescriptorImageInfo DescriptorImage{};
    DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DescriptorImage.imageView = view;
    DescriptorImage.sampler = sampler;
    return DescriptorImage;
}

VkShaderModule GraphicsPipeline::ReadShaderFile(const std::string& filename)
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

void GraphicsPipeline::BuildDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
{
    SubmitDescriptorSet(buildGraphicsPipelineInfo.DescriptorBindingList);
}

void GraphicsPipeline::BuildShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
{
    VkVertexInputBindingDescription bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    if (buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderMesh ||
        buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderWireFrame)
    {
        bindingDescription = MeshVertex::getBindingDescription();
        attributeDescriptions = MeshVertex::getAttributeDescriptions();
    }
    else if (buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderLine)
    {
        bindingDescription = LineVertex::getBindingDescription();
        attributeDescriptions = LineVertex::getAttributeDescriptions();
    }

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    if (buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderMesh)
    {
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    }
    else if (buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderLine)
    {
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
    }
    else if (buildGraphicsPipelineInfo.PipelineRendererType == PipelineRendererTypeEnum::kRenderWireFrame)
    {
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
    }

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = buildGraphicsPipelineInfo.sampleCount;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = static_cast<uint32_t>(buildGraphicsPipelineInfo.ColorAttachments.size());
    colorBlending.pAttachments = buildGraphicsPipelineInfo.ColorAttachments.data();
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;

    if (buildGraphicsPipelineInfo.ConstBufferSize != 0)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = buildGraphicsPipelineInfo.ConstBufferSize;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    }

    if (vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout.");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(buildGraphicsPipelineInfo.PipelineShaderStageList.size());
    pipelineInfo.pStages = buildGraphicsPipelineInfo.PipelineShaderStageList.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = ShaderPipelineLayout;
    pipelineInfo.renderPass = buildGraphicsPipelineInfo.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    auto a = vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline);

    if (a != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline.");
    }
}

VkPipelineShaderStageCreateInfo GraphicsPipeline::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = shaderStages;
    vertShaderStageInfo.module = ReadShaderFile(filename);
    vertShaderStageInfo.pName = "main";

    return vertShaderStageInfo;
}