#pragma once
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"

class RayTraceRenderer
{
private:
	AccelerationStructureBuffer TopLevelAccelerationStructure{};
    VulkanBuffer InstancesBuffer;

    void SetUpCommandBuffers();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    VkCommandBuffer RayTraceCommandBuffer;

    RayTraceRenderer();
    ~RayTraceRenderer();

    void StartUp();
    void SetUpTopLevelAccelerationStructure();
    void Draw();
    void RebuildSwapChain();
    void Destroy();
};

