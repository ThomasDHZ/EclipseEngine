#include "Scene.h"
#include "MeshRenderer.h"
std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
  /*  nlohmann::json json;
    Vec4 a = Vec4()*/

//to_json(json, *this);
//std::cout << json << std::endl;
//from_json(json, temp);

    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial"));
    material->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Diffuse.png");
    material->LoadAlphaMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Alpha.png");
    MaterialManager::AddMaterial(material);

    std::shared_ptr<Material> material2 = std::make_shared<Material>(Material("TestMaterial2"));
    material2->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/space-cruiser-panels2_albedo.png");
    MaterialManager::AddMaterial(material2);


    std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject", glm::vec2(0.0f), 0));
    std::shared_ptr<GameObject> obj2 = std::make_shared<GameObject>(GameObject("Testobject2", glm::vec2(2.0f, 0.0f), 1));
    std::shared_ptr<GameObject> obj3 = std::make_shared<GameObject>(GameObject("Testobject3", glm::vec2(1.0f), 0));
    std::shared_ptr<GameObject> obj4 = std::make_shared<GameObject>(GameObject("Testobject4", glm::vec2(2.0f, 1.0f), 2));
    //std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject3D", glm::vec3(5.0f)));
    
    auto a = obj->GetComponentByType(ComponentType::kSpriteRenderer);
    auto b = static_cast<MeshRenderer*>(a.get());
    b->SetMaterial(material2);

   // std::shared_ptr<GameObject> obj2 = std::make_shared<GameObject>(GameObject("Testobject3D2", glm::vec3(2.0f, 0.0f, 0.0f)));
    auto a2 = obj2->GetComponentByType(ComponentType::kSpriteRenderer);
    auto b2 = static_cast<MeshRenderer*>(a2.get());
    b2->SetMaterial(material);

  //  std::shared_ptr<GameObject> obj3 = std::make_shared<GameObject>(GameObject("Testobject3D3", glm::vec3(1.0f)));
    auto a3 = obj3->GetComponentByType(ComponentType::kSpriteRenderer);
    auto b3 = static_cast<MeshRenderer*>(a3.get());
    b3->SetMaterial(material2);

  //  std::shared_ptr<GameObject> obj4 = std::make_shared<GameObject>(GameObject("Testobject3D4", glm::vec3(2.0f, 1.0f, 0.0f)));
    auto a4 = obj4->GetComponentByType(ComponentType::kSpriteRenderer);
    auto b4 = static_cast<MeshRenderer*>(a4.get());
    b4->SetMaterial(material);

    //Transform3D temp;
    //nlohmann::json json;

    //obj->ToJson(json);
    //std::cout << json << std::endl;
    //GameObject asdf = GameObject(json);
    //temp = Transform3D(json);

    GameObjectManager::AddGameObject(obj);
    GameObjectManager::AddGameObject(obj2);
    GameObjectManager::AddGameObject(obj3);
    GameObjectManager::AddGameObject(obj4);

    renderer2D.StartUp();
    blinnPhongRenderer.StartUp();
}

Scene::~Scene()
{
}

void Scene::StartUp()
{
}

void Scene::Update()
{
    if (VulkanRenderer::UpdateRendererFlag)
    {
        RebuildRenderers();
    }

    auto time = glfwGetTime();
    GameObjectManager::Update(time);

    camera2.Update(time);

    sceneProperites.CameraPos = camera2.GetPosition();
    sceneProperites.view = camera2.GetViewMatrix();
    sceneProperites.proj = camera2.GetProjectionMatrix();
    sceneProperites.Timer = time;
}

void Scene::ImGuiUpdate()
{

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    
    const auto objList = GameObjectManager::GetGameObjectList();
    
    for (int x = 0; x < objList.size(); x++)
    {
        if (objList[x]->GetComponentByType(ComponentType::kTransform2D))
        {
            auto transformz = objList[x]->GetComponentByType(ComponentType::kTransform2D);
            auto Transform = static_cast<Transform2D*>(transformz.get());
            ImGui::SliderFloat2(objList[x]->GetObjectName().c_str(), &Transform->Position.x, 0.0f, 100.0f);
        }
    }
    for (int x = 0; x < objList.size(); x++)
    {
        if (objList[x]->GetComponentByType(ComponentType::kTransform3D))
        {
            auto transformz = objList[x]->GetComponentByType(ComponentType::kTransform3D);
            auto Transform = static_cast<Transform3D*>(transformz.get());
            ImGui::SliderFloat3(objList[x]->GetObjectName().c_str(), &Transform->Position.x, 0.0f, 100.0f);
        }
    }

    if (objList.size() == 4)
    {
        GameObjectManager::RemoveGameObject(objList[3]);
    }

 /*   if (renderPass2D.renderedTexture->ImGuiDescriptorSet != nullptr)
    {
        ImGui::Image(renderPass2D.renderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }*/
}

void Scene::RebuildRenderers()
{
    auto objList = GameObjectManager::GetGameObjectList();

    renderer2D.RebuildRenderers();
    blinnPhongRenderer.RebuildRenderers();
    InterfaceRenderPass::RebuildSwapChain();

    VulkanRenderer::UpdateRendererFlag = false;
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


    //renderer2D.Draw(sceneProperites, CommandBufferSubmitList);
    blinnPhongRenderer.Draw(sceneProperites, CommandBufferSubmitList);
    InterfaceRenderPass::Draw();
    CommandBufferSubmitList.emplace_back(InterfaceRenderPass::ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

    result = VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RebuildRenderers();
        return;
    }
}

void Scene::Destroy()
{
    GameObjectManager::Destory();
    
    renderer2D.Destroy();
    blinnPhongRenderer.Destroy();
    InterfaceRenderPass::Destroy();
}
