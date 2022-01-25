#include "Scene.h"

Scene::Scene()
{
   
}

Scene::~Scene()
{
}

void Scene::Update()
{
    VulkanRenderer::StartDraw();
    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

    //imGuiRenderPass.Draw();
    //CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);
    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
