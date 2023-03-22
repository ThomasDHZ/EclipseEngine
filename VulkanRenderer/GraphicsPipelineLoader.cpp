#include "GraphicsPipelineLoader.h"

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



VkPipelineShaderStageCreateInfo JsonGraphicsPipeline::LoadVKPipelineShaderStageCreateInfo(nlohmann::json& json)
{
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo{};
    PipelineShaderStageCreateInfo.sType = json["sType"];
    PipelineShaderStageCreateInfo.stage = json["stage"];
    PipelineShaderStageCreateInfo.module = ReadShaderFile(json["shaderFile"]);
    PipelineShaderStageCreateInfo.pName = "main";
    // JsonConverter::to_json(json["pSpecializationInfo"], PipelineShaderStageCreateInfo.pSpecializationInfo);
    //JsonConverter::to_json(json["pNext"], PipelineShaderStageCreateInfo.pNext);

    return PipelineShaderStageCreateInfo;
}

VkDescriptorPool JsonGraphicsPipeline::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo)
{
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
    poolInfo.pPoolSizes = DescriptorPoolInfo.data();
    poolInfo.maxSets = static_cast<uint32_t>(DescriptorPoolInfo.size());

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return descriptorPool;
}

void JsonGraphicsPipeline::SaveDescriptorLayoutSet(nlohmann::json& json, VkDescriptorSetLayoutBinding descriptorSetLayoutBinding)
{
    JsonConverter::to_json(json["binding"], descriptorSetLayoutBinding.binding);
    JsonConverter::to_json(json["descriptorType"], descriptorSetLayoutBinding.descriptorType);
    JsonConverter::to_json(json["descriptorCount"], descriptorSetLayoutBinding.descriptorCount);
    JsonConverter::to_json(json["stageFlags"], descriptorSetLayoutBinding.stageFlags);
    //JsonConverter::to_json(json["pImmutableSamplers"], pImmutableSamplers);
}



void JsonGraphicsPipeline::SaveDescriptorPoolSize(nlohmann::json& json, VkDescriptorPoolSize descriptorPoolSize)
{
    JsonConverter::to_json(json["type"], descriptorPoolSize.type);
    JsonConverter::to_json(json["descriptorCount"], descriptorPoolSize.descriptorCount);
}

VkDescriptorPoolSize JsonGraphicsPipeline::LoadDescriptorPoolSize(nlohmann::json& json)
{
    VkDescriptorPoolSize descriptorPoolSize{};
    descriptorPoolSize.type = json["type"];
    descriptorPoolSize.descriptorCount = json["descriptorCount"];
    
    return descriptorPoolSize;
}

VkDescriptorSetLayoutBinding JsonGraphicsPipeline::LoadDescriptorLayoutSet(nlohmann::json& json)
{
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
    descriptorSetLayoutBinding.binding = json["binding"];
    descriptorSetLayoutBinding.descriptorType = json["descriptorType"];
    descriptorSetLayoutBinding.descriptorCount = json["descriptorCount"];
    descriptorSetLayoutBinding.stageFlags = json["stageFlags"];
    //descriptorSetLayoutBinding.pImmutableSamplers = json["pImmutableSamplers"];

    return descriptorSetLayoutBinding;
}

void JsonGraphicsPipeline::SaveVKPipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename)
{
    JsonConverter::to_json(json["flags"], PipelineShaderStageCreateInfo.flags);
    JsonConverter::to_json(json["sType"], PipelineShaderStageCreateInfo.sType);
    JsonConverter::to_json(json["stage"], PipelineShaderStageCreateInfo.stage);
    JsonConverter::to_json(json["shaderFile"], filename);
    //JsonConverter::to_json(json["pName"], "main");
    //JsonConverter::to_json(json["pSpecializationInfo"], PipelineShaderStageCreateInfo.pSpecializationInfo);
    //JsonConverter::to_json(json["pNext"], PipelineShaderStageCreateInfo.pNext);
}

void JsonGraphicsPipeline::SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo)
{
    JsonConverter::to_json(json["sType"], descriptorPoolCreateInfo.sType);
    //JsonConverter::to_json(json["pNext"], descriptorPool.pNext);
    JsonConverter::to_json(json["flags"], descriptorPoolCreateInfo.flags);
    JsonConverter::to_json(json["maxSets"], descriptorPoolCreateInfo.maxSets);
    JsonConverter::to_json(json["poolSizeCount"], descriptorPoolCreateInfo.poolSizeCount);
    //JsonConverter::to_json(json["pPoolSizes"], descriptorPool.pPoolSizes);
}

VkDescriptorPool JsonGraphicsPipeline::LoadCreateDescriptorPool(nlohmann::json& json)
{
    VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo = {};
    DescriptorPoolCreateInfo.sType = json["sType"];
   // DescriptorPoolCreateInfo.pNext = json["pNext"];
    DescriptorPoolCreateInfo.flags = json["flags"];
    DescriptorPoolCreateInfo.maxSets = json["maxSets"];
    DescriptorPoolCreateInfo.poolSizeCount = json["poolSizeCount"];
    //DescriptorPoolCreateInfo.pPoolSizes = json["pPoolSizes"];

    VkDescriptorPool DescriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &DescriptorPoolCreateInfo, nullptr, &DescriptorPool) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return DescriptorPool;
}

JsonGraphicsPipeline::JsonGraphicsPipeline()
{
}

JsonGraphicsPipeline::~JsonGraphicsPipeline()
{
}

void JsonGraphicsPipeline::LoadGraphicsPipeline(const std::string& filename)
{
    nlohmann::json json;
}

void JsonGraphicsPipeline::SaveGraphicsPipeline(VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename)
{
    nlohmann::json json;
    SaveVKPipelineShaderStageCreateInfo(json, PipelineShaderStageCreateInfo, filename);
}
