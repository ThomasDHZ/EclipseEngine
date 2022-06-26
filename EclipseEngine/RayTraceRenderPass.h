#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "SceneManager.h"

class RayTraceRenderPass : public RenderPass
{
private:
    void SetUpCommandBuffers();
    void BuildRenderPassPipelines();

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;
    VkCommandBuffer RayTraceCommandBuffer;

    RayTraceRenderPass();
    ~RayTraceRenderPass();

    void StartUp();
    VkCommandBuffer Draw();
    void RebuildSwapChain();
    void Destroy();
};

