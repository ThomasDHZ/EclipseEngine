#include "RayTraceRenderer.h"

RayTraceRenderer::RayTraceRenderer() : BaseRenderPass()
{
}

RayTraceRenderer::~RayTraceRenderer()
{
}

void RayTraceRenderer::StartUp()
{
    rayTraceRenderPass.StartUp();
    FrameBufferRenderer.StartUp(rayTraceRenderPass.RayTracedTexture);
}

void RayTraceRenderer::RebuildSwapChain()
{
    rayTraceRenderPass.RebuildSwapChain();
    FrameBufferRenderer.RebuildSwapChain(rayTraceRenderPass.RayTracedTexture);
}

void RayTraceRenderer::GUIUpdate()
{
}

void RayTraceRenderer::Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
    rayTraceRenderPass.Draw(sceneProperties);
    CommandBufferSubmitList.emplace_back(rayTraceRenderPass.GetCommandBuffer());

    FrameBufferRenderer.Draw();
    CommandBufferSubmitList.emplace_back(FrameBufferRenderer.GetCommandBuffer());
}

void RayTraceRenderer::Destroy()
{
    rayTraceRenderPass.Destroy();
    FrameBufferRenderer.Destroy();
}

