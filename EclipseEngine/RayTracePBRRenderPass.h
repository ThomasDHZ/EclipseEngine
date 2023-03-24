#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"
#include "LightManager.h"
#include <GLTFSceneManager.h>

class RayTracePBRRenderPass : public RenderPass
{
private:
    void SetUpCommandBuffers();
    void BuildRenderPassPipelines();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;
    VkCommandBuffer RayTraceCommandBuffer;

    RayTracePBRRenderPass();
    ~RayTracePBRRenderPass();

    void StartUp();
    VkCommandBuffer Draw();
    void RebuildSwapChain();
    void Destroy();
};
