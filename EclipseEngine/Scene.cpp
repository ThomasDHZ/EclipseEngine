#include "Scene.h"

Scene::Scene()
{
    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    GameObject obj = GameObject("Testobject", glm::vec2(0.0f), 0);
    obj.AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
    objList.emplace_back(obj);

    GameObject obj2 = GameObject("Testobject", glm::vec2(2.0f, 0.0f), 0);
    obj2.AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
    objList.emplace_back(obj2);

    imGuiRenderPass.StartUp();
    renderPass2D.StartUp(objList[0], objList[1]);
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

    {
        auto spriteRenderer = objList[0].GetComponentByType(ComponentType::kSpriteRenderer);
        auto transform2D = objList[0].GetComponentByType(ComponentType::kTransform2D);
        if (spriteRenderer != nullptr &&
            transform2D != nullptr)
        {
            SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
            Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

            MeshProperties2 meshProps = {};
            meshProps.MeshTransform = transform->Transform;
            sprite->MeshProperties.Update(meshProps);
        }
    }
    {
        auto spriteRenderer = objList[1].GetComponentByType(ComponentType::kSpriteRenderer);
        auto transform2D = objList[1].GetComponentByType(ComponentType::kTransform2D);
        if (spriteRenderer != nullptr &&
            transform2D != nullptr)
        {
            SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
            Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

            MeshProperties2 meshProps = {};
            meshProps.MeshTransform = transform->Transform;
            sprite->MeshProperties.Update(meshProps);
        }
    }

    camera.Update(time);

    sceneProperites.CameraPos = camera.GetPosition();
    sceneProperites.view = camera.GetViewMatrix();
    sceneProperites.proj = camera.GetProjectionMatrix();
    sceneProperites.Timer = time;
}

void Scene::ImGuiUpdate()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    {
        auto comp = objList[0].GetComponentByType(ComponentType::kTransform2D);
        auto Transform = static_cast<Transform2D*>(comp.get());
        ImGui::SliderFloat3("sdf", &Transform->Position.x, 0.0f, 100.0f);
    }
    {
        auto comp = objList[1].GetComponentByType(ComponentType::kTransform2D);
        auto Transform = static_cast<Transform2D*>(comp.get());
        ImGui::SliderFloat3("sdf2", &Transform->Position.x, 0.0f, 100.0f);
    }
    if (renderPass2D.renderedTexture->ImGuiDescriptorSet != nullptr)
    {
        ImGui::Image(renderPass2D.renderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
}

void Scene::RebuildRenderers()
{
    renderPass2D.RebuildSwapChain(objList[0], objList[1]);
    frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
    imGuiRenderPass.RebuildSwapChain();
}

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VkResult result = VulkanRenderer::StartDraw();
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RebuildRenderers();
        return;
    }

    renderPass2D.Draw(objList, sceneProperites);
    CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

    frameBufferRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());

    imGuiRenderPass.Draw();
    CommandBufferSubmitList.emplace_back(imGuiRenderPass.ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

    result = VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RebuildRenderers();
        return;
    }
}

void Scene::Destroy()
{
}
