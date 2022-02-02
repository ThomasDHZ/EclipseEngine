#include "Scene.h"

Scene::Scene()
{
    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    GameObject obj = GameObject();
    obj.AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
    objList.emplace_back(obj);

    imGuiRenderPass.StartUp();
    renderPass2D.StartUp();
	frameBufferRenderPass.StartUp(renderPass2D.renderedTexture);

    texture = Texture2D("C:/Users/dotha/source/repos/VulkanGraphics/texture/forrest_ground_01_ao_4k.jpg", VK_FORMAT_R8G8B8A8_SRGB);
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
    
    for (auto obj : objList)
    {
        obj.Update(time);
    }

    camera2.Update(time);

    sceneProperites.CameraPos = camera2.GetPosition();
    sceneProperites.view = camera2.GetViewMatrix();
    sceneProperites.proj = camera2.GetProjectionMatrix();
    sceneProperites.Timer = time;
}

void Scene::ImGuiUpdate()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   
    if (renderPass2D.renderedTexture->ImGuiDescriptorSet != nullptr)
    {
        ImGui::Image(renderPass2D.renderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
}

void Scene::RebuildRenderers()
{
    renderPass2D.RebuildSwapChain();
    frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
    imGuiRenderPass.RebuildSwapChain();
}

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VulkanRenderer::StartDraw();


    renderPass2D.Draw(objList, sceneProperites);
    CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

    imGuiRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
