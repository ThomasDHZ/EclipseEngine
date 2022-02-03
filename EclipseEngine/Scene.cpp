#include "Scene.h"

std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject", glm::vec2(0.0f), 0));
    std::shared_ptr<GameObject> obj2 = std::make_shared<GameObject>(GameObject("Testobject2", glm::vec2(2.0f, 0.0f), 0));
    std::shared_ptr<GameObject> obj3 = std::make_shared<GameObject>(GameObject("Testobject3", glm::vec2(1.0f), 0));
    std::shared_ptr<GameObject> obj4 = std::make_shared<GameObject>(GameObject("Testobject4", glm::vec2(2.0f, 1.0f), 0));

    GameObjectManager::AddGameObject(obj);
    GameObjectManager::AddGameObject(obj2);
    GameObjectManager::AddGameObject(obj3);
    GameObjectManager::AddGameObject(obj4);

    imGuiRenderPass.StartUp();
    renderPass2D.StartUp(GameObjectManager::GetGameObjectsByID(obj->GetGameObjectID()), GameObjectManager::GetGameObjectsByID(obj2->GetGameObjectID()));
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
    
    for (auto obj : GameObjectManager::GetGameObjectList())
    {
        obj->Update(time);

        {
            auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
            auto transform2D = obj->GetComponentByType(ComponentType::kTransform2D);
            if (spriteRenderer != nullptr &&
                transform2D != nullptr)
            {
                SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
                Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

                MeshProperties meshProps = {};
                meshProps.MeshTransform = transform->Transform;
                sprite->mesh.MeshProperties.Update(meshProps);
            }
        }
        {
            auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
            auto transform2D = obj->GetComponentByType(ComponentType::kTransform2D);
            if (spriteRenderer != nullptr &&
                transform2D != nullptr)
            {
                SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
                Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

                MeshProperties meshProps = {};
                meshProps.MeshTransform = transform->Transform;
                sprite->mesh.MeshProperties.Update(meshProps);
            }
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
    auto objList = GameObjectManager::GetGameObjectList();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    {
        auto comp = objList[0]->GetComponentByType(ComponentType::kTransform2D);
        auto Transform = static_cast<Transform2D*>(comp.get());
        ImGui::SliderFloat3("sdf", &Transform->Position.x, 0.0f, 100.0f);
    }
    {
        auto comp = objList[1]->GetComponentByType(ComponentType::kTransform2D);
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
    auto objList = GameObjectManager::GetGameObjectList();

    renderPass2D.RebuildSwapChain(objList[0], objList[1]);
    frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
    imGuiRenderPass.RebuildSwapChain();
}

void Scene::Draw()
{
    auto objList = GameObjectManager::GetGameObjectList();
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VkResult result = VulkanRenderer::StartDraw();
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RebuildRenderers();
        return;
    }

    renderPass2D.Draw(sceneProperites);
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
