#pragma once
#include "RenderPass.h"
#include "AccelerationStructureBuffer.h"
#include "RenderedColorTexture.h"
#include "RayTracingPipeline.h"
#include "GameObject.h"
#include "GLTFSceneManager.h"

class RayTracePBRRenderPass : public RenderPass
{
private:
    VkDescriptorPool DescriptorPool;
    VkDescriptorSet DescriptorSet;
    VkDescriptorSetLayout DescriptorSetLayout;
    std::vector<VkDescriptorSetLayout> DescriptorSetLayoutList;

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
