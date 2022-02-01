#include "Scene.h"

Scene::Scene()
{
    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    GameObject obj = GameObject();
    obj.AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
    objList.emplace_back(obj);

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

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VulkanRenderer::StartDraw();

  /*  imGuiRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);*/
    //obj.Draw();
    renderPass2D.Draw(objList, sceneProperites);
    CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

    VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
}
