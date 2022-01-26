#include "Scene.h"

Scene::Scene()
{
    imGuiRenderPass.StartUp();
	frameBufferRenderPass.StartUp();
}

Scene::~Scene()
{
}

void Scene::StartUp()
{
}

void Scene::Update()
{

}

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VulkanRenderer::StartDraw();
    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

  
    //imGuiRenderPass.Draw();
    //CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);
    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
