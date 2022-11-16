#pragma once
#include <GraphicsPipeline.h>
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "UniformBuffer.h"
#include "GameObjectManager.h"

class RayTracingPipeline : public GraphicsPipeline
{
private:
    VulkanBuffer RaygenShaderBindingTable;
    VulkanBuffer MissShaderBindingTable;
    VulkanBuffer HitShaderBindingTable;

    void SetUpDescriptorBindings(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
    void SetUpPipeline(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList);
    void SetUpShaderBindingTable(std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList);

public:

    RayTracingPipeline();
    ~RayTracingPipeline();

    void SetUp(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
    void UpdateGraphicsPipeLine(std::vector<VkPipelineShaderStageCreateInfo>& ShaderList, std::vector<VkRayTracingShaderGroupCreateInfoKHR>& RayTraceShaderList, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
    void Destroy();

    VkBuffer GetRaygenShaderBindingTable() { return RaygenShaderBindingTable.GetBuffer(); }
    VkBuffer GetMissShaderBindingTable() { return MissShaderBindingTable.GetBuffer(); }
    VkBuffer GetHitShaderBindingTable() { return HitShaderBindingTable.GetBuffer(); }
};