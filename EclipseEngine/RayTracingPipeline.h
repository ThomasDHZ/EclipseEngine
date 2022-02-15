#pragma once
#include "GraphicsPipeline.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "UniformBuffer.h"
#include "GameObjectManager.h"

class RayTracingPipeline : public GraphicsPipeline
{
private:
    VulkanBuffer raygenShaderBindingTable;
    VulkanBuffer missShaderBindingTable;
    VulkanBuffer hitShaderBindingTable;
    VulkanBuffer rayTraceSceneProperties;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR  rayTracingPipelineProperties{};
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures{};

    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaders{};

    void SetUpDescriptorBindings(AccelerationStructureBuffer& TopLevelAccelerationStructure);
    void SetUpPipeline();
    void SetUpShaderBindingTable();

public:
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;

    RayTracingPipeline();
    ~RayTracingPipeline();

    void SetUp(AccelerationStructureBuffer& TopLevelAccelerationStructure);
    void UpdateGraphicsPipeLine(AccelerationStructureBuffer& TopLevelAccelerationStructure);
};