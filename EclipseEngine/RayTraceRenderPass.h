#pragma once
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"

class RayTraceRenderPass
{
private:
	AccelerationStructureBuffer TopLevelAccelerationStructure{};
    VulkanBuffer InstancesBuffer;

    void SetUpCommandBuffers();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    VkCommandBuffer RayTraceCommandBuffer;

    std::shared_ptr<RenderedColorTexture> RayTracedTexture;

    RayTraceRenderPass();
    ~RayTraceRenderPass();

    void StartUp();
    void SetUpTopLevelAccelerationStructure();
    void Draw(SceneProperties& sceneProperties);
    void RebuildSwapChain();
    void Destroy();
};

