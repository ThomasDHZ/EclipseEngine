#pragma once
#include "BaseRenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"

class RayTraceRenderPass : public BaseRenderPass
{
private:
	AccelerationStructureBuffer TopLevelAccelerationStructure{};
    VulkanBuffer InstancesBuffer;

    void SetUpCommandBuffers();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;

    RayTraceRenderPass();
    ~RayTraceRenderPass();

    void StartUp();
    void SetUpTopLevelAccelerationStructure();
    void Draw(SceneProperties& sceneProperties);
    void RebuildSwapChain();
    void Destroy();
};

