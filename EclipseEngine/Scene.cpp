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
    imGuiRenderPass.Draw();
    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
