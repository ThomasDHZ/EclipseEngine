#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"
#include "LightManager.h"
#include "TopLevelAccelerationStructureManager.h"
#include "SceneManager.h"

class RaytraceHybridPass : public RenderPass
{
private:
    void SetUpCommandBuffers();
    void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> cubeMapTexture);

public:
    std::shared_ptr<RayTracingPipeline> RayTracePipeline;
    std::shared_ptr<RenderedColorTexture> RenderedShadowTexture;
    VkCommandBuffer RayTraceCommandBuffer;

    RaytraceHybridPass();
    ~RaytraceHybridPass();
    static std::string BaseShaderFilePath;
    void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMapTexture);
    VkCommandBuffer Draw();
    void Destroy();
};
