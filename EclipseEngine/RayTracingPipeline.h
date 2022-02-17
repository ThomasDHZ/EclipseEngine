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
    VulkanBuffer RayTraceSceneProperties;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR  RayTracingPipelineProperties{};
    VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures{};

    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaders{};

    void SetUpDescriptorBindings(AccelerationStructureBuffer& TopLevelAccelerationStructure);
    void SetUpPipeline();
    void SetUpShaderBindingTable();

public:

    RayTracingPipeline();
    ~RayTracingPipeline();

    void SetUp(AccelerationStructureBuffer& TopLevelAccelerationStructure);
    void UpdateGraphicsPipeLine(AccelerationStructureBuffer& TopLevelAccelerationStructure);

    VkBuffer GetRaygenShaderBindingTable() { return RaygenShaderBindingTable.GetBuffer(); }
    VkBuffer GetMissShaderBindingTable() { return MissShaderBindingTable.GetBuffer(); }
    VkBuffer GetHitShaderBindingTable() { return HitShaderBindingTable.GetBuffer(); }
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR GetRayTracingPipelineProperties() { return RayTracingPipelineProperties; }
};