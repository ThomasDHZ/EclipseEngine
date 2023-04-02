#include "..\VulkanRenderer\GLTF_GraphicsPipeline.h"
#include "..\VulkanRenderer\GLTF_GraphicsPipeline.h"
#include "JsonGraphicsPipeline.h"
#include "SceneManager.h"

std::string JsonGraphicsPipeline::BaseShaderFilePath = "../Shaders/";
std::string JsonGraphicsPipeline::BasePipelineFilePath = "../Pipelines/";

JsonGraphicsPipeline::JsonGraphicsPipeline()
{
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::shared_ptr<GLTF_Temp_Model> model, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
{
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, model, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::~JsonGraphicsPipeline()
{
}

VkShaderModule JsonGraphicsPipeline::ReadShaderFile(const std::string& filename)
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
        throw std::runtime_error("Failed to create shader module.");
    }

    return shaderModule;
}

VkPipelineShaderStageCreateInfo JsonGraphicsPipeline::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = shaderStages;
    vertShaderStageInfo.module = ReadShaderFile(filename);
    vertShaderStageInfo.pName = "main";

    return vertShaderStageInfo;
}

VkDescriptorPool JsonGraphicsPipeline::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount)
{
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
    poolInfo.pPoolSizes = DescriptorPoolInfo.data();
    poolInfo.maxSets = 300;

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return descriptorPool;
}

VkDescriptorSet JsonGraphicsPipeline::CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet DescriptorSets;
    if (vkAllocateDescriptorSets(VulkanRenderer::GetDevice(), &allocInfo, &DescriptorSets) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets.");
    }

    return DescriptorSets;
}


VkWriteDescriptorSet JsonGraphicsPipeline::AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure)
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

VkWriteDescriptorSet JsonGraphicsPipeline::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet JsonGraphicsPipeline::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet JsonGraphicsPipeline::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet JsonGraphicsPipeline::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet JsonGraphicsPipeline::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = dstArrayElement;
    TextureDescriptor.descriptorType = descriptorType;
    TextureDescriptor.descriptorCount = 1;
    TextureDescriptor.pImageInfo = &TextureImageInfo;
    return TextureDescriptor;
}

VkWriteDescriptorSet JsonGraphicsPipeline::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet TextureDescriptor = {};
    TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    TextureDescriptor.dstSet = DescriptorSet;
    TextureDescriptor.dstBinding = BindingNumber;
    TextureDescriptor.dstArrayElement = dstArrayElement;
    TextureDescriptor.descriptorType = descriptorType;
    TextureDescriptor.descriptorCount = static_cast<uint32_t>(TextureImageInfo.size());
    TextureDescriptor.pImageInfo = TextureImageInfo.data();
    return TextureDescriptor;
}

VkWriteDescriptorSet JsonGraphicsPipeline::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = dstArrayElement;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = 1;
    BufferDescriptor.pBufferInfo = &BufferInfo;
    return BufferDescriptor;
}

VkWriteDescriptorSet JsonGraphicsPipeline::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet BufferDescriptor = {};
    BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    BufferDescriptor.dstSet = DescriptorSet;
    BufferDescriptor.dstBinding = BindingNumber;
    BufferDescriptor.dstArrayElement = dstArrayElement;
    BufferDescriptor.descriptorType = descriptorType;
    BufferDescriptor.descriptorCount = static_cast<uint32_t>(BufferInfoList.size());
    BufferDescriptor.pBufferInfo = BufferInfoList.data();
    return BufferDescriptor;
}

void JsonGraphicsPipeline::AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    DescriptorSetBinding.AccelerationStructureDescriptor = accelerationStructure;
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = TextureImageInfo;
    DescriptorSetBinding.Count = TextureImageInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void JsonGraphicsPipeline::SaveDescriptorBindingLayout(nlohmann::json& json, VkDescriptorType descriptorType, DescriptorBindingPropertiesEnum descriptorBindingPropertiesEnum)
{
    JsonConverter::to_json(json["descriptorType"], descriptorType);
    JsonConverter::to_json(json["bindingType"], descriptorBindingPropertiesEnum);
}

void JsonGraphicsPipeline::SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], descriptorPool.pNext);
    JsonConverter::to_json(json["flags"], descriptorPoolCreateInfo.flags);
    JsonConverter::to_json(json["maxSets"], descriptorPoolCreateInfo.maxSets);
    JsonConverter::to_json(json["poolSizeCount"], descriptorPoolCreateInfo.poolSizeCount);
    //JsonConverter::to_json(json["pPoolSizes"], descriptorPool.pPoolSizes);
}

void JsonGraphicsPipeline::SaveBufferDescriptorSet(nlohmann::json& json, VkWriteDescriptorSet writeDescriptorSet)
{
    //JsonConverter::to_json(json["pNext"], writeDescriptorSet.pNext);
    //JsonConverter::to_json(json["dstSet"], writeDescriptorSet.dstSet);
    JsonConverter::to_json(json["dstBinding"], writeDescriptorSet.dstBinding);
    JsonConverter::to_json(json["dstArrayElement"], writeDescriptorSet.dstArrayElement);
    JsonConverter::to_json(json["descriptorCount"], writeDescriptorSet.descriptorCount);
    JsonConverter::to_json(json["descriptorType"], writeDescriptorSet.descriptorType);
    //JsonConverter::to_json(json["pImageInfo"], writeDescriptorSet.pImageInfo);
    //JsonConverter::to_json(json["pBufferInfo"], writeDescriptorSet.pBufferInfo);
    //JsonConverter::to_json(json["pTexelBufferView"], writeDescriptorSet.pTexelBufferView);
}

void JsonGraphicsPipeline::SavePipelineDepthStencilStateCreateInfo(nlohmann::json& json, VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineDepthStencilStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineDepthStencilStateCreateInfo.flags);
    JsonConverter::to_json(json["depthTestEnable"], pipelineDepthStencilStateCreateInfo.depthTestEnable);
    JsonConverter::to_json(json["depthWriteEnable"], pipelineDepthStencilStateCreateInfo.depthWriteEnable);
    JsonConverter::to_json(json["depthCompareOp"], pipelineDepthStencilStateCreateInfo.depthCompareOp);
    JsonConverter::to_json(json["depthBoundsTestEnable"], pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable);
    JsonConverter::to_json(json["stencilTestEnable"], pipelineDepthStencilStateCreateInfo.stencilTestEnable);
    //JsonConverter::to_json(json["front"], pipelineDepthStencilStateCreateInfo.front);
    //JsonConverter::to_json(json["back"], pipelineDepthStencilStateCreateInfo.back);
    JsonConverter::to_json(json["minDepthBounds"], pipelineDepthStencilStateCreateInfo.minDepthBounds);
    JsonConverter::to_json(json["maxDepthBounds"], pipelineDepthStencilStateCreateInfo.maxDepthBounds);
}

void JsonGraphicsPipeline::SavePipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename)
{
    JsonConverter::to_json(json["flags"], PipelineShaderStageCreateInfo.flags);
    JsonConverter::to_json(json["stage"], PipelineShaderStageCreateInfo.stage);
    JsonConverter::to_json(json["shaderFile"], filename);
    json["pName"] = "main";
    //JsonConverter::to_json(json["pSpecializationInfo"], PipelineShaderStageCreateInfo.pSpecializationInfo);
    //JsonConverter::to_json(json["pNext"], PipelineShaderStageCreateInfo.pNext);
}

void JsonGraphicsPipeline::SavePipelineInputAssemblyStateCreateInfo(nlohmann::json& json, VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineInputAssemblyStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineInputAssemblyStateCreateInfo.flags);
    JsonConverter::to_json(json["topology"], pipelineInputAssemblyStateCreateInfo.topology);
    JsonConverter::to_json(json["primitiveRestartEnable"], pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable);
}

void JsonGraphicsPipeline::SavePipelineViewportStateCreateInfo(nlohmann::json& json, VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineViewportStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineViewportStateCreateInfo.flags);
    JsonConverter::to_json(json["viewportCount"], pipelineViewportStateCreateInfo.viewportCount);
    //JsonConverter::to_json(json["pViewports"], pipelineViewportStateCreateInfo.pViewports);
    JsonConverter::to_json(json["scissorCount"], pipelineViewportStateCreateInfo.scissorCount);
    // JsonConverter::to_json(json["pScissors"], pipelineViewportStateCreateInfo.pScissors);
}

void JsonGraphicsPipeline::SavePipelineRasterizationStateCreateInfo(nlohmann::json& json, VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineRasterizationStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineRasterizationStateCreateInfo.flags);
    JsonConverter::to_json(json["depthClampEnable"], pipelineRasterizationStateCreateInfo.depthClampEnable);
    JsonConverter::to_json(json["rasterizerDiscardEnable"], pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable);
    JsonConverter::to_json(json["polygonMode"], pipelineRasterizationStateCreateInfo.polygonMode);
    JsonConverter::to_json(json["cullMode"], pipelineRasterizationStateCreateInfo.cullMode);
    JsonConverter::to_json(json["frontFace"], pipelineRasterizationStateCreateInfo.frontFace);
    JsonConverter::to_json(json["depthBiasEnable"], pipelineRasterizationStateCreateInfo.depthBiasEnable);
    JsonConverter::to_json(json["depthBiasConstantFactor"], pipelineRasterizationStateCreateInfo.depthBiasConstantFactor);
    JsonConverter::to_json(json["depthBiasClamp"], pipelineRasterizationStateCreateInfo.depthBiasClamp);
    JsonConverter::to_json(json["depthBiasSlopeFactor"], pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor);
    JsonConverter::to_json(json["lineWidth"], pipelineRasterizationStateCreateInfo.lineWidth);
}

void JsonGraphicsPipeline::SavePipelineMultisampleStateCreateInfo(nlohmann::json& json, VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineMultisampleStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineMultisampleStateCreateInfo.flags);
    JsonConverter::to_json(json["sampleShadingEnable"], pipelineMultisampleStateCreateInfo.sampleShadingEnable);
    JsonConverter::to_json(json["minSampleShading"], pipelineMultisampleStateCreateInfo.minSampleShading);
    //JsonConverter::to_json(json["pSampleMask"], pipelineMultisampleStateCreateInfo.pSampleMask);
    JsonConverter::to_json(json["alphaToCoverageEnable"], pipelineMultisampleStateCreateInfo.alphaToCoverageEnable);
    JsonConverter::to_json(json["alphaToOneEnable"], pipelineMultisampleStateCreateInfo.alphaToOneEnable);
}

void JsonGraphicsPipeline::SavePipelineColorBlendStateCreateInfo(nlohmann::json& json, VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo)
{
    JsonConverter::to_json(json["flags"], pipelineColorBlendStateCreateInfo.flags);
    JsonConverter::to_json(json["logicOpEnable"], pipelineColorBlendStateCreateInfo.logicOpEnable);
    JsonConverter::to_json(json["logicOp"], pipelineColorBlendStateCreateInfo.logicOp);
    JsonConverter::to_json(json["blendConstants"][0], pipelineColorBlendStateCreateInfo.blendConstants[0]);
    JsonConverter::to_json(json["blendConstants"][1], pipelineColorBlendStateCreateInfo.blendConstants[1]);
    JsonConverter::to_json(json["blendConstants"][2], pipelineColorBlendStateCreateInfo.blendConstants[2]);
    JsonConverter::to_json(json["blendConstants"][3], pipelineColorBlendStateCreateInfo.blendConstants[3]);
}

VkDescriptorPool JsonGraphicsPipeline::LoadCreateDescriptorPool(nlohmann::json& json)
{
    VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo = {};
    DescriptorPoolCreateInfo.pNext = nullptr;
    DescriptorPoolCreateInfo.flags = json["flags"];
    DescriptorPoolCreateInfo.maxSets = json["maxSets"];
    DescriptorPoolCreateInfo.poolSizeCount = json["poolSizeCount"];
    DescriptorPoolCreateInfo.pPoolSizes = nullptr;

    VkDescriptorPool DescriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &DescriptorPoolCreateInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return DescriptorPool;
}

VkPipelineLayoutCreateInfo JsonGraphicsPipeline::LoadPipelineLayoutCreateInfo()
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    return pipelineLayoutCreateInfo;
}

VkPipelineShaderStageCreateInfo JsonGraphicsPipeline::LoadPipelineShaderStageCreateInfo(nlohmann::json& json)
{
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo{};
    PipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    PipelineShaderStageCreateInfo.stage = json["stage"];
    PipelineShaderStageCreateInfo.module = ReadShaderFile(json["shaderFile"]);
    PipelineShaderStageCreateInfo.pName = "main";
    PipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
    PipelineShaderStageCreateInfo.pNext = nullptr;

    return PipelineShaderStageCreateInfo;
}

VkPipelineDepthStencilStateCreateInfo JsonGraphicsPipeline::LoadPipelineDepthStencilStateCreateInfo(nlohmann::json& json)
{
    VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = {};

    pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineDepthStencilStateCreateInfo.pNext = nullptr;
    pipelineDepthStencilStateCreateInfo.depthTestEnable = json["depthTestEnable"];
    pipelineDepthStencilStateCreateInfo.depthWriteEnable = json["depthWriteEnable"];
    pipelineDepthStencilStateCreateInfo.depthCompareOp = json["depthCompareOp"];
    pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = json["depthBoundsTestEnable"];
    pipelineDepthStencilStateCreateInfo.stencilTestEnable = json["stencilTestEnable"];
    //pipelineDepthStencilStateCreateInfo.front = json["front"];
   // pipelineDepthStencilStateCreateInfo.back = json["back"];
    pipelineDepthStencilStateCreateInfo.minDepthBounds = json["minDepthBounds"];
    pipelineDepthStencilStateCreateInfo.maxDepthBounds = json["maxDepthBounds"];

    return pipelineDepthStencilStateCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo JsonGraphicsPipeline::LoadPipelineInputAssemblyStateCreateInfo(nlohmann::json& json)
{
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
    pipelineInputAssemblyStateCreateInfo.flags = json["flags"];
    pipelineInputAssemblyStateCreateInfo.topology = json["topology"];
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = json["primitiveRestartEnable"];

    return pipelineInputAssemblyStateCreateInfo;
}

VkPipelineViewportStateCreateInfo JsonGraphicsPipeline::LoadPipelineViewportStateCreateInfo(nlohmann::json& json)
{
    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.pNext = nullptr;
    pipelineViewportStateCreateInfo.flags = json["flags"];
    pipelineViewportStateCreateInfo.viewportCount = json["viewportCount"];
    pipelineViewportStateCreateInfo.pViewports = nullptr;
    pipelineViewportStateCreateInfo.viewportCount = json["viewportCount"];
    pipelineViewportStateCreateInfo.pViewports = nullptr;
    pipelineViewportStateCreateInfo.pScissors = nullptr;

    return pipelineViewportStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo  JsonGraphicsPipeline::LoadPipelineRasterizationStateCreateInfo(nlohmann::json& json)
{
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.pNext = nullptr;
    pipelineRasterizationStateCreateInfo.depthClampEnable = json["depthClampEnable"];
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = json["rasterizerDiscardEnable"];
    pipelineRasterizationStateCreateInfo.polygonMode = json["polygonMode"];
    pipelineRasterizationStateCreateInfo.cullMode = json["cullMode"];
    pipelineRasterizationStateCreateInfo.frontFace = json["frontFace"];
    pipelineRasterizationStateCreateInfo.depthBiasEnable = json["depthBiasEnable"];
    pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = json["depthBiasConstantFactor"];
    pipelineRasterizationStateCreateInfo.depthBiasClamp = json["depthBiasClamp"];
    pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = json["depthBiasSlopeFactor"];
    pipelineRasterizationStateCreateInfo.lineWidth = json["lineWidth"];

    return pipelineRasterizationStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo JsonGraphicsPipeline::LoadPipelineMultisampleStateCreateInfo(nlohmann::json& json, VkSampleCountFlagBits sampleCount)
{
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = nullptr;
    pipelineMultisampleStateCreateInfo.flags = json["flags"];
    pipelineMultisampleStateCreateInfo.rasterizationSamples = sampleCount;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = json["sampleShadingEnable"];
    pipelineMultisampleStateCreateInfo.minSampleShading = json["minSampleShading"];
    pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = json["alphaToCoverageEnable"];
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = json["alphaToOneEnable"];

    return pipelineMultisampleStateCreateInfo;
}

VkPipelineColorBlendStateCreateInfo JsonGraphicsPipeline::LoadPipelineColorBlendStateCreateInfo(nlohmann::json& json, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments)
{
    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.flags = json["flags"];
    pipelineColorBlendStateCreateInfo.logicOpEnable = json["logicOpEnable"];
    pipelineColorBlendStateCreateInfo.logicOp = json["logicOp"];
    pipelineColorBlendStateCreateInfo.attachmentCount = static_cast<uint32_t>(ColorAttachments.size());
    pipelineColorBlendStateCreateInfo.pAttachments = ColorAttachments.data();
    pipelineColorBlendStateCreateInfo.blendConstants[0] = json["blendConstants"][0];
    pipelineColorBlendStateCreateInfo.blendConstants[1] = json["blendConstants"][1];
    pipelineColorBlendStateCreateInfo.blendConstants[2] = json["blendConstants"][2];
    pipelineColorBlendStateCreateInfo.blendConstants[3] = json["blendConstants"][3];

    return pipelineColorBlendStateCreateInfo;
}

void JsonGraphicsPipeline::LoadDescriptorSets(nlohmann::json& json, std::shared_ptr<GLTF_Temp_Model> model)
{
    std::vector<DescriptorBindingPropertiesEnum> BindingList;
    std::vector<VkDescriptorType> DescriptorList;
    for (int x = 0; x < json.size(); x++)
    {
        BindingList.emplace_back(json[x]["bindingType"]);
        DescriptorList.emplace_back(json[x]["descriptorType"]);
    }

    std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
    for (int x = 0; x < BindingList.size(); x++)
    {
        switch (BindingList[x])
        {
            case kModelTransformDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)model->GetMeshPropertiesBuffer().size() }); break;
            case kMeshPropertiesDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kAlbedoMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kNormalMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kMetallicRoughnessMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kAmbientOcclusionMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kAlphaMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kDepthMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kBRDFMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kIrradianceMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kPrefilterMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kCubeMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kEnvironmentDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kSunLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size() }); break;
            case kDirectionalLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size() }); break;
            case kPointLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size() }); break;
            case kSpotLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size() }); break;
        }
    }
    DescriptorPool = CreateDescriptorPool(descriptorPoolSizeList, 1);

    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
    for (int x = 0; x < BindingList.size(); x++)
    {
        switch (BindingList[x])
        {
            case kModelTransformDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x, DescriptorList[x], (uint32_t)model->GetMeshPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kMeshPropertiesDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kAlbedoMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kNormalMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kMetallicRoughnessMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kAmbientOcclusionMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kAlphaMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kDepthMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kBRDFMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kIrradianceMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kPrefilterMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kCubeMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kEnvironmentDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kSunLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kDirectionalLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kPointLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kSpotLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
        }
    }

    DescriptorSetLayout = GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding);
    if (model != nullptr)
    {
        for (int y = 0; y < model->MaterialList.size(); y++)
        {
            std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
            for (int x = 0; x < BindingList.size(); x++)
            {
                switch (BindingList[x])
                {
                case kModelTransformDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, model->GetMeshPropertiesBuffer()); break;
                case kMeshPropertiesDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, model->MeshList[0]->GetTransformMatrixBuffer()[0]); break;
                case kAlbedoMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetAlbedoMapDescriptor()); break;
                case kNormalMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetNormalMapDescriptor()); break;
                case kMetallicRoughnessMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetMetallicRoughnessMapDescriptor()); break;
                case kAmbientOcclusionMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetAmbientOcclusionMapDescriptor()); break;
                case kAlphaMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetAlphaMapDescriptor()); break;
                case kDepthMapDescriptor:  AddTextureDescriptorSetBinding(DescriptorBindingList, x, model->MaterialList[y]->GetDepthMapDescriptor()); break;
                case kBRDFMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetBRDFMapDescriptor()); break;
                case kIrradianceMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetIrradianceMapDescriptor()); break;
                case kPrefilterMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetPrefilterMapDescriptor()); break;
                case kCubeMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetCubeMapDescriptor()); break;
                case kEnvironmentDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetEnvironmentMapDescriptor()); break;
                case kSunLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetSunLightPropertiesBuffer()); break;
                case kDirectionalLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetDirectionalLightPropertiesBuffer()); break;
                case kPointLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetPointLightPropertiesBuffer()); break;
                case kSpotLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetSpotLightPropertiesBuffer()); break;
                }
            }
            model->MaterialList[y]->descriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

            std::vector<VkWriteDescriptorSet> writeDescriptorSet;
            for (auto& DescriptorBinding : DescriptorBindingList)
            {
                switch (DescriptorBinding.DescriptorType)
                {
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: writeDescriptorSet.emplace_back(AddBufferDescriptorSet(model->MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType)); break;
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: writeDescriptorSet.emplace_back(AddTextureDescriptorSet(model->MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType)); break;
                case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: writeDescriptorSet.emplace_back(AddAccelerationBuffer(model->MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor)); break;
                }
            }
            vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);

            for (int x = 0; x < model->MeshList.size(); x++)
            {
                model->MeshList[x]->MaterialList = model->MaterialList;
            }
        }
    }
    else
    {
        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        for (int x = 0; x < BindingList.size(); x++)
        {
            switch (BindingList[x])
            {
                 case kEnvironmentDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetEnvironmentMapDescriptor()); break;

                case kBRDFMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetBRDFMapDescriptor()); break;
                case kCubeMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetCubeMapDescriptor()); break;
            }
        }
        CubeMapDescriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

        std::vector<VkWriteDescriptorSet> writeDescriptorSet;
        for (auto& DescriptorBinding : DescriptorBindingList)
        {
            switch (DescriptorBinding.DescriptorType)
            {
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: writeDescriptorSet.emplace_back(AddBufferDescriptorSet(CubeMapDescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType)); break;
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: writeDescriptorSet.emplace_back(AddTextureDescriptorSet(CubeMapDescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType)); break;
            case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: writeDescriptorSet.emplace_back(AddAccelerationBuffer(CubeMapDescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor)); break;
            }
        }
        vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
    }
}

void JsonGraphicsPipeline::LoadGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::shared_ptr<GLTF_Temp_Model> model, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
{
    std::string SceneInfo;
    std::ifstream SceneFile;
    SceneFile.open(BasePipelineFilePath + filePath);
    if (SceneFile.is_open())
    {
        while (!SceneFile.eof())
        {
            getline(SceneFile, SceneInfo);
        }
    }
    else std::cout << "Unable to open file";
    SceneFile.close();

    nlohmann::json json = nlohmann::json::parse(SceneInfo);

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    LoadDescriptorSets(json["DescriptorBindingLayout"], model);

    if (ShaderPipeline == nullptr)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(VertexBindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();

        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = LoadPipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"]);
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = LoadPipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"]);
        VkPipelineViewportStateCreateInfo viewportState = LoadPipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"]);
        VkPipelineRasterizationStateCreateInfo rasterizer = LoadPipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"]);
        VkPipelineMultisampleStateCreateInfo multisampling = LoadPipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], samplecount);
        VkPipelineColorBlendStateCreateInfo colorBlending = LoadPipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], ColorAttachments);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        if (sizeofConstBuffer != 0)
        {
            VkPushConstantRange pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeofConstBuffer;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        }

        if (vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout.");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(PipelineShaderStageList.size());
        pipelineInfo.pStages = PipelineShaderStageList.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = ShaderPipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkResult result = vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline.");
        }
    }
}

void JsonGraphicsPipeline::SaveGraphicsPipeline(const char* fileName, nlohmann::json& json)
{
    std::ofstream pipelineFile(BasePipelineFilePath + fileName);
    pipelineFile << json;
    pipelineFile.close();
}

void JsonGraphicsPipeline::BuildAndSaveShaderPipeLine(nlohmann::json& json, BuildGraphicsPipelineInfo& buildPipelineInfo, VkDescriptorSetLayout descriptorSetLayout)
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = buildPipelineInfo.VertexDescription.VertexTopology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    SavePipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"], inputAssembly);

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    SavePipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"], viewportState);

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.lineWidth = 1.0f;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.cullMode = buildPipelineInfo.VertexDescription.CullMode;
    rasterizer.polygonMode = buildPipelineInfo.VertexDescription.PolygonMode;
    rasterizer.depthBiasEnable = VK_FALSE;
    SavePipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"], rasterizer);

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = buildPipelineInfo.RenderPassDescription.sampleCount;
    SavePipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], multisampling);

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.attachmentCount = static_cast<uint32_t>(buildPipelineInfo.RenderPassDescription.ColorAttachments.size());
    colorBlending.pAttachments = buildPipelineInfo.RenderPassDescription.ColorAttachments.data();
    SavePipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], colorBlending);
}

void JsonGraphicsPipeline::Destroy()
{
    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
    vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VulkanRenderer::GetDevice(), DescriptorSetLayout, nullptr);

    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;
    DescriptorPool = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;
}