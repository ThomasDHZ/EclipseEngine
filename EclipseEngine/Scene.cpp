#include "Scene.h"

Scene::Scene()
{
    texture = Texture("C:/Users/dotha/source/repos/VulkanGraphics/texture/pbr/gold/albedo.png", VK_FORMAT_R8G8B8A8_SRGB);
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
