#include "JsonGraphicsPipeline.h"
#include "SceneManager.h"

std::string JsonGraphicsPipeline::BaseShaderFilePath = "../Shaders/";
std::string JsonGraphicsPipeline::BasePipelineFilePath = "../Pipelines/";

JsonGraphicsPipeline::JsonGraphicsPipeline()
{
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const char* filePath, VkRenderPass renderPass, std::vector<GLTF_Temp_Model>& model, uint32_t modelIndex, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
{
    LoadGraphicsPipeline(filePath, renderPass, model, modelIndex, ColorAttachments, samplecount, sizeofConstBuffer);
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

void JsonGraphicsPipeline::SaveDescriptorPoolSize(nlohmann::json& json, VkDescriptorPoolSize descriptorPoolSize)
{
    JsonConverter::to_json(json["type"], descriptorPoolSize.type);
    JsonConverter::to_json(json["descriptorCount"], descriptorPoolSize.descriptorCount);
}

void JsonGraphicsPipeline::SaveDescriptorLayoutSet(nlohmann::json& json, VkDescriptorSetLayoutBinding descriptorSetLayoutBinding)
{
    JsonConverter::to_json(json["binding"], descriptorSetLayoutBinding.binding);
    JsonConverter::to_json(json["descriptorType"], descriptorSetLayoutBinding.descriptorType);
    JsonConverter::to_json(json["descriptorCount"], descriptorSetLayoutBinding.descriptorCount);
    JsonConverter::to_json(json["stageFlags"], descriptorSetLayoutBinding.stageFlags);
    //JsonConverter::to_json(json["pImmutableSamplers"], pImmutableSamplers);
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
    //JsonConverter::to_json(json["pName"], "main");
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
    descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

    return descriptorSetLayoutBinding;
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

void JsonGraphicsPipeline::LoadGraphicsPipeline(const char* filePath, VkRenderPass renderPass, std::vector<GLTF_Temp_Model>& model, uint32_t modelIndex, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
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

    std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[modelIndex].GetMeshPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetSunLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetDirectionalLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetPointLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetSpotLightPropertiesBuffer().size() });
    DescriptorPool = CreateDescriptorPool(descriptorPoolSizeList, 1);

    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[modelIndex].GetMeshPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 , VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 7, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 8, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 9, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 10, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 11, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 12, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 13, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 14, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model[1].GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    DescriptorSetLayout = GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding);

    for (int y = 0; y < model[modelIndex].MaterialList.size(); y++)
    {
        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, model[modelIndex].GetMeshPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, model[modelIndex].MeshList[0]->GetTransformMatrixBuffer()[0]);
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 2, model[modelIndex].MaterialList[y]->GetAlbedoMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, model[modelIndex].MaterialList[y]->GetNormalMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, model[modelIndex].MaterialList[y]->GetMetallicRoughnessMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, model[modelIndex].MaterialList[y]->GetAmbientOcclusionMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, model[modelIndex].MaterialList[y]->GetAlphaMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 7, model[modelIndex].MaterialList[y]->GetDepthMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 8, SceneManager::GetBRDFMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 9, SceneManager::GetIrradianceMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 10, SceneManager::GetPrefilterMapDescriptor());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 11, model[1].GetSunLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 12, model[1].GetDirectionalLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 13, model[1].GetPointLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 14, model[1].GetSpotLightPropertiesBuffer());
        model[modelIndex].MaterialList[y]->descriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

        std::vector<VkWriteDescriptorSet> writeDescriptorSet;
        for (auto& DescriptorBinding : DescriptorBindingList)
        {
            if (DescriptorBinding.BufferDescriptor.size() > 0)
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddBufferDescriptorSet(model[modelIndex].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
            }
            else if (DescriptorBinding.TextureDescriptor.size() > 0)
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddTextureDescriptorSet(model[modelIndex].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
            }
            else
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddAccelerationBuffer(model[modelIndex].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
            }
        }
        vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
        for (int x = 0; x < model[modelIndex].MeshList.size(); x++)
        {
            model[modelIndex].MeshList[x]->MaterialList = model[modelIndex].MaterialList;
        }
    }


    if (ShaderPipeline == nullptr)
    {
        const std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
        const std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();

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

void JsonGraphicsPipeline::Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model& model, uint32_t descriptorsetIndex, uint32_t descriptorsetcount)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    model.Draw(commandBuffer, ShaderPipelineLayout, descriptorsetIndex, descriptorsetcount);
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