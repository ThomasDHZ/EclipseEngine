#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"
#include "LightManager.h"
#include "TopLevelAccelerationStructureManager.h"
#include "SceneManager.h"


class RayTraceRenderPass : public RenderPass
{
private:
    void SetUpCommandBuffers();
    void BuildRenderPassPipelines();

    std::string BaseShaderFilePath = "../Shaders/";
public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RayTracedTexture;
    VkCommandBuffer RayTraceCommandBuffer;

    RayTraceRenderPass();
    ~RayTraceRenderPass();

    void BuildRenderPass();
    VkCommandBuffer Draw();
    void Destroy();
};

