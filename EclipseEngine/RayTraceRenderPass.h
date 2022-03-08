#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"

class RayTraceRenderPass : public RenderPass
{
private:
	AccelerationStructureBuffer TopLevelAccelerationStructure{};
    VulkanBuffer InstancesBuffer;

    void SetUpCommandBuffers();
    void BuildRenderPassPipelines();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;
    VkCommandBuffer RayTraceCommandBuffer;

    RayTraceRenderPass();
    ~RayTraceRenderPass();

    void StartUp();
    void SetUpTopLevelAccelerationStructure();
    void Draw(SceneProperties& sceneProperties);
    void RebuildSwapChain();
    void Destroy();
};

