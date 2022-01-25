#include "Scene.h"

Scene::Scene()
{
	frameBufferRenderPass.StartUp();
}

Scene::~Scene()
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
    //CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::CMDIndex]);
    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
