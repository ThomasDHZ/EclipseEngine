#include "RayTracingPipeline.h"

RayTracingPipeline::RayTracingPipeline() : GraphicsPipeline()
{
}

RayTracingPipeline::~RayTracingPipeline()
{
}

void RayTracingPipeline::SetUp(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
    SetUpDescriptorBindings(DescriptorBindingList);
    SetUpPipeline(ShaderList, RayTraceShaderList);
    SetUpShaderBindingTable(RayTraceShaderList);
}

void RayTracingPipeline::SetUpDescriptorBindings(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
    SubmitDescriptorSet(DescriptorBindingList);
}

void RayTracingPipeline::SetUpPipeline(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SceneProperties);

    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo = {};
    PipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    PipelineLayoutCreateInfo.setLayoutCount = 1;
    PipelineLayoutCreateInfo.pSetLayouts = &DescriptorSetLayout;
    PipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    PipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
    vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &PipelineLayoutCreateInfo, nullptr, &ShaderPipelineLayout);

    VkRayTracingPipelineCreateInfoKHR RayTracingPipeline = {};
    RayTracingPipeline.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    RayTracingPipeline.stageCount = static_cast<uint32_t>(ShaderList.size());
    RayTracingPipeline.pStages = ShaderList.data();
    RayTracingPipeline.groupCount = static_cast<uint32_t>(RayTraceShaderList.size());
    RayTracingPipeline.pGroups = RayTraceShaderList.data();
    RayTracingPipeline.maxPipelineRayRecursionDepth = 2;
    RayTracingPipeline.layout = ShaderPipelineLayout;
    VkResult result = VulkanRenderer::vkCreateRayTracingPipelinesKHR(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &RayTracingPipeline, nullptr, &ShaderPipeline);

    for (auto shader : ShaderList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void RayTracingPipeline::UpdateGraphicsPipeLine(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
    GraphicsPipeline::UpdateGraphicsPipeLine();
    RaygenShaderBindingTable.DestoryBuffer();
    MissShaderBindingTable.DestoryBuffer();
    HitShaderBindingTable.DestoryBuffer();

    SetUpDescriptorBindings(DescriptorBindingList);
    SetUpPipeline(ShaderList, RayTraceShaderList);
    SetUpShaderBindingTable(RayTraceShaderList);
}

void RayTracingPipeline::Destroy()
{
    RaygenShaderBindingTable.DestoryBuffer();
    MissShaderBindingTable.DestoryBuffer();
    HitShaderBindingTable.DestoryBuffer();

    GraphicsPipeline::Destroy();
}

void RayTracingPipeline::SetUpShaderBindingTable(std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList)
{
    const uint32_t handleSize = GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleSize;
    const uint32_t handleSizeAligned = GraphicsDevice::GetAlignedSize(handleSize, GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleAlignment);
    const uint32_t groupCount = static_cast<uint32_t>(RayTraceShaderList.size());
    const uint32_t sbtSize = groupCount * handleSizeAligned;

    std::vector<uint8_t> shaderHandleStorage(sbtSize);
    VulkanRenderer::vkGetRayTracingShaderGroupHandlesKHR(VulkanRenderer::GetDevice(), ShaderPipeline, 0, groupCount, sbtSize, shaderHandleStorage.data());

    RaygenShaderBindingTable.CreateBuffer(shaderHandleStorage.data(), handleSize, VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    MissShaderBindingTable.CreateBuffer(shaderHandleStorage.data() + handleSizeAligned, handleSize * 2, VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    HitShaderBindingTable.CreateBuffer(shaderHandleStorage.data() + (handleSizeAligned * 3), handleSize * 3, VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}