#include "Scene.h"
#include "EnvironmentTexture.h"

std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{


    camera = OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 1.0f);
    camera2 = PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f));

    SceneManager::LoadScene("../Scenes/example.txt");

    CubeMapLayout cubeMapfiles;
    cubeMapfiles.Left = "../texture/skybox/right.jpg";
    cubeMapfiles.Right = "../texture/skybox/left.jpg";
    cubeMapfiles.Top = "../texture/skybox/top.jpg";
    cubeMapfiles.Bottom = "../texture/skybox/bottom.jpg";
    cubeMapfiles.Front = "../texture/skybox/back.jpg";
    cubeMapfiles.Back = "../texture/skybox/front.jpg";
    TextureManager::LoadCubeMapTexture(cubeMapfiles);

    TextureManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/newport_loft.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

     //std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject", "../Models/Sponza/Sponza.gltf"));
   //  //std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject", "../Models/vulkanscene_shadow.obj"));

   //  //std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject("Testobject", "../Models/Cerberus/Cerberus_LP.FBX"));

    //auto dLight = DirectionalLightBuffer{};
    //dLight.diffuse = glm::vec3(0.2f);
    //dLight.specular = glm::vec3(0.5f);

    //LightManager::AddDirectionalLight(dLight);

    //PointLightBuffer plight = PointLightBuffer();
    //plight.position = glm::vec3(-10.0f, 10.0f, 10.0f);
    //plight.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    //plight.specular = glm::vec3(1.0f);

    //PointLightBuffer plight2 = PointLightBuffer();
    //plight2.position = glm::vec3(10.0f, 10.0f, 10.0f);
    //plight2.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    //plight2.specular = glm::vec3(1.0f);

    //PointLightBuffer plight3 = PointLightBuffer();
    //plight3.position = glm::vec3(-10.0f, -10.0f, 10.0f);
    //plight3.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    //plight3.specular = glm::vec3(1.0f);

    //PointLightBuffer plight4 = PointLightBuffer();
    //plight4.position = glm::vec3(10.0f, -10.0f, 10.0f);
    //plight4.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    //plight4.specular = glm::vec3(1.0f);

    //LightManager::AddPointLight(plight);
    //LightManager::AddPointLight(plight2);
    //LightManager::AddPointLight(plight3);
    //LightManager::AddPointLight(plight4);

    //SceneManager::SaveScene("../Scenes/example.txt");


    //renderer2D.StartUp();
//blinnPhongRenderer.StartUp();
//hybridRenderer.StartUp();
    pbrRenderer.StartUp();
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        rayTraceRenderer.StartUp();
    }
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
    MaterialManager::Update();
    MeshRendererManager::Update();
    ModelManager::Update();
    LightManager::Update();

    camera2.Update(time);

    sceneProperites.CameraPos = camera2.GetPosition();
    sceneProperites.view = camera2.GetViewMatrix();
    sceneProperites.proj = camera2.GetProjectionMatrix();
    sceneProperites.DirectionalLightCount = LightManager::GetDirectionalLightCount();
    sceneProperites.PointLightCount = LightManager::GetPointLightCount();
    sceneProperites.SpotLightCount = LightManager::GetSpotLightCount();
    sceneProperites.Timer = time;

    cubeMapInfo.view = glm::mat4(glm::mat3(camera2.GetViewMatrix()));
    cubeMapInfo.proj = glm::perspective(glm::radians(camera2.GetZoom()), VulkanRenderer::GetSwapChainResolution().width / (float)VulkanRenderer::GetSwapChainResolution().height, 0.1f, 100.0f);
    cubeMapInfo.proj[1][1] *= -1;

    if (GraphicsDevice::IsRayTracingFeatureActive() &&
        GraphicsDevice::IsRayTracerActive())
    {
        rayTraceRenderer.Update();
    }
    else
    {
        //renderer2D.Update();
      //  hybridRenderer.Update();
     // blinnPhongRenderer.Update();
       pbrRenderer.Update();
        
    }


}

void Scene::ImGuiUpdate()
{
    //ImGui::Begin("VRAM Viewer");
    //{
    //    for (int x = 0; x <= TextureManager::GetTexture2DList().size() - 1; x++)
    //    {
    //        ImGui::LabelText("Texture ID: " + TextureManager::GetTexture2DList()[x]->GetTextureID(), "Model Transform");
    //        ImGui::Image(TextureManager::GetTexture2DList()[x]->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //    }
    //}
    //ImGui::End();

    ImGui::Begin("File Window");
    if (ImGui::Button("Save"))
    {
        SceneManager::SaveScene("../Scenes/example.txt");
    }

    if (ImGui::Button("Load"))
    {
        vkDeviceWaitIdle(VulkanRenderer::GetDevice());
        SceneManager::LoadScene("../Scenes/example.txt");
    }
    ImGui::End();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    const auto objList = GameObjectManager::GetGameObjectList();
    ImGui::Checkbox("Wireframe Mode", &VulkanRenderer::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        ImGui::Checkbox("RayTrace Mode", &GraphicsDevice::RayTracingActive);
    }

    for (auto& model : ModelManager::GetModelList())
    {

        ImGui::LabelText("", "Model Transform");
        ImGui::SliderFloat3("Model position ", &model->GetModelPositionPtr()->x, -100.0f, 100.0f);
        ImGui::SliderFloat3("Model rotation ", &model->GetModelRotationPtr()->x, 0.0f, 360.0f);
        ImGui::SliderFloat3("Model scale ", &model->GetModelScalePtr()->x, 0.0f, 1.0f);
    }

    MeshRendererManager::GUIUpdate();
    LightManager::GUIUpdate();

   // ImGui::Image(pbrRenderer.irradianceRenderPass.texture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.TangentTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.BiTangentTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.TBNormalTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.NormalTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.AlbedoTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.SpecularTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //ImGui::Image(hybridRenderer.GBufferRenderPass.BloomTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));

    VulkanRenderer::ImGUILayerActive = ImGui::IsWindowHovered();
}

void Scene::RebuildRenderers()
{
    MeshRendererManager::Update();

    //renderer2D.RebuildRenderers();
   // blinnPhongRenderer.RebuildRenderers();
    //hybridRenderer.RebuildRenderers();
    pbrRenderer.RebuildRenderers();
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        rayTraceRenderer.RebuildSwapChain();
    }
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

    if (GraphicsDevice::IsRayTracingFeatureActive() && 
        GraphicsDevice::IsRayTracerActive())
    {
        rayTraceRenderer.Draw(sceneProperites, CommandBufferSubmitList);
        
    }
    else
    {
       // renderer2D.Draw(sceneProperites, CommandBufferSubmitList);
     //  blinnPhongRenderer.Draw(sceneProperites, cubeMapInfo, CommandBufferSubmitList);
       // hybridRenderer.Draw(sceneProperites, CommandBufferSubmitList);
       pbrRenderer.Draw(sceneProperites, cubeMapInfo, CommandBufferSubmitList);
    }
 
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
    GameObjectManager::Destroy();

    //renderer2D.Destroy();
   // blinnPhongRenderer.Destroy();
    //hybridRenderer.Destroy();
    pbrRenderer.Destroy();
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        rayTraceRenderer.Destroy();
    }
}
