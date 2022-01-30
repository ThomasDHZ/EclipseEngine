#include "Scene.h"

Scene::Scene()
{
    camera = Camera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    mesh = Mesh(vertices, indices);
    texture = Texture("C:/Users/dotha/source/repos/VulkanGraphics/texture/pbr/gold/albedo.png", VK_FORMAT_R8G8B8A8_SRGB);
    imGuiRenderPass.StartUp();
    renderPass2D.StartUp();
	frameBufferRenderPass.StartUp(renderPass2D.renderedTexture);
}

Scene::~Scene()
{
}

void Scene::StartUp()
{
}

void Scene::Update()
{
    auto time = glfwGetTime();
    camera.Update(time);

    sceneProperites.CameraPos = camera.GetPosition();
    sceneProperites.view = camera.GetViewMatrix();
    sceneProperites.proj = camera.GetProjectionMatrix();
    sceneProperites.Timer = time;
}

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VulkanRenderer::StartDraw();

  /*  imGuiRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);*/

    renderPass2D.Draw(mesh, sceneProperites);
    CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
