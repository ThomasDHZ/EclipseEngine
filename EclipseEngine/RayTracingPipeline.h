#pragma once
#include "GraphicsPipeline.h"
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

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR  RayTracingPipelineProperties{};
    VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures{};

    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaders{};

    void SetUpDescriptorBindings(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
    void SetUpPipeline();
    void SetUpShaderBindingTable();

public:

    RayTracingPipeline();
    ~RayTracingPipeline();

    void SetUp(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
    void UpdateGraphicsPipeLine(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);

    VkBuffer GetRaygenShaderBindingTable() { return RaygenShaderBindingTable.GetBuffer(); }
    VkBuffer GetMissShaderBindingTable() { return MissShaderBindingTable.GetBuffer(); }
    VkBuffer GetHitShaderBindingTable() { return HitShaderBindingTable.GetBuffer(); }
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR GetRayTracingPipelineProperties() { return RayTracingPipelineProperties; }
};