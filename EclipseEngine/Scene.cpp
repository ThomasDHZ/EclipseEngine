#include "Scene.h"
#include "EnvironmentTexture.h"
#include "MusicPlayer.h"
#include "Mesh2D.h"
#include "LineMesh2D.h"
#include "LineRenderer2D.h"
#include "GridRenderer2D.h"
#include "LineRenderer3D.h"
#include "GridRenderer3D.h"
#include "SpriteRenderer.h"
#include "ModelRenderer.h"
#include "BillBoardMeshRenderer.h"

std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    SceneManager::sceneType = SceneType::kBlinnPhong;

   // SceneManager::activeCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 10.5f));
    SceneManager::activeCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

    std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial", MaterialTypeEnum::kMaterialBlinnPhong));
    material->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Diffuse.png");
    material->LoadAlphaMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Alpha.png");
    MaterialManager::AddMaterial(material);


    std::shared_ptr<Material> material2 = std::make_shared<Material>(Material("TestMaterial2", MaterialTypeEnum::kMaterialBlinnPhong));
    material2->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/space-cruiser-panels2_albedo.png");
    MaterialManager::AddMaterial(material2);

 /*    glm::vec2 StartPoint = glm::vec2(0.0f);
        glm::vec2 EndPoint = glm::vec2(5.0f);
        glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.3f);
;     obj5 = std::make_shared<GameObject2D>(GameObject2D("Testobject5", StartPoint, EndPoint, Color));
       GameObjectManager::AddGameObject(obj5);*/

   /*    glm::vec3 StartPoint2 = glm::vec3(1.0f);
       glm::vec3 EndPoint2 = glm::vec3(5.0f);
       glm::vec4 Color2 = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
       auto   obj6 = std::make_shared<LineRenderer3D>(LineRenderer3D("Testobject5", StartPoint2, EndPoint2, Color2));*/

       //glm::vec2 StartPoint = glm::vec2(0.0f);
       //glm::vec2 EndPoint = glm::vec2(5.0f);
       //glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.3f);
       //;   auto  obj6 = std::make_shared<LineRenderer2D>(LineRenderer2D("Testobject5", StartPoint, EndPoint, Color));
       //GameObjectManager::AddGameObject(obj6);

    //std::vector<MeshVertex> vertices = {
    //    {{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
    //    {{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
    //    {{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
    //    {{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
    //};

    // std::vector<uint32_t> indices = {
    //    0, 1, 2, 2, 3, 0
    //};
     //std::shared_ptr<SpriteRenderer> obj = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject", material, glm::vec2(0.0f), 0));
     //std::shared_ptr<SpriteRenderer> obj2 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject2", material2, glm::vec2(1.0f, 0.0f), 0));
     //std::shared_ptr<SpriteRenderer> obj3 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject3", material, glm::vec2(2.0f, 0.0f), 0));
     //std::shared_ptr<SpriteRenderer> obj4 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject4", material2, glm::vec2(3.0f, 0.0f), 0));

    //GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject5", glm::vec3(0.0f), glm::vec3(5.0f, 5.0f, 5.0f))));
  //  std::shared_ptr<GameObject3D> obj5 = std::make_shared<GameObject3D>(GameObject3D("Testobject6", 5, .25));
   //  std::shared_ptr<GridRenderer3D> obj5 = std::make_shared<GridRenderer3D>(GridRenderer3D("Testobject6", 50, 1.0f));
    //GameObjectManager::AddGameObject(obj5);

    //int width = 500;
    //int height = 500;
    //float length = 10.0f;
    //float radius = 0.5f;
    //std::vector<LineVertex3D> VertexList;
    //for (uint32_t y = 0; y < height; y++)
    //{
    //    for (uint32_t x = 0; x < width; x++)
    //    {
    //        glm::vec2 coord = { (float)x / width, (float)y / height };
    //        coord = coord * 2.0f - 1.0f;

    //        uint8_t r = (uint8_t)(coord.x * 255.0f);
    //        uint8_t g = (uint8_t)(coord.y * 255.0f);

    //        glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    //        glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

    //        float a = glm::dot(rayDirection, rayDirection);
    //        float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    //        float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    //        //Quadratic forumla discriminat
    //        //b^2 - 4ac;

    //        float discriminant = (b * b) - 4.0f * a * c;

    //        if (discriminant >= 0.0f)
    //        {
    //            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(1.0f, 0.0f, 0.8f, 1.0f)));
    //            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(1.0f, 0.0f, 0.8, 1.0f)));
    //        }
    //        else
    //        {
    //            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.0f, 0.0f, 01.0f, 0.02f)));
    //            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.02f)));
    //        }
    //    }
    //}
    //auto line = std::make_shared<LineRenderer3D>(LineRenderer3D("Testobject5", VertexList));


//   SceneManager::LoadScene("../Scenes/example.txt");

    CubeMapLayout cubeMapfiles;
    cubeMapfiles.Left = "../texture/skybox/right.jpg";
    cubeMapfiles.Right = "../texture/skybox/left.jpg";
    cubeMapfiles.Top = "../texture/skybox/top.jpg";
    cubeMapfiles.Bottom = "../texture/skybox/bottom.jpg";
    cubeMapfiles.Front = "../texture/skybox/back.jpg";
    cubeMapfiles.Back = "../texture/skybox/front.jpg";
    TextureManager::LoadCubeMapTexture(cubeMapfiles);

    SceneManager::environmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/newport_loft.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

   // GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/RayReflectionTest.obj")));

    std::shared_ptr<Material> material3 = std::make_shared<Material>(Material("Char", MaterialTypeEnum::kMaterialBlinnPhong));
    material3->LoadAlbedoMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/CharmanderDiffuse.png");
    material3->LoadDiffuseMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/CharmanderDiffuse.png");
    material3->LoadNormalMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/charmanderSculptedFinal.jpg");
  //  material3->LoadAmbientOcclusionMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/charmanderScupltedcharmandeNoser4-AO_u0_v0.bmp");
    MaterialManager::AddMaterial(material3);



    auto b= std::make_shared<BillBoardMeshRenderer>(BillBoardMeshRenderer("Testobject2", glm::vec2(1.0f)));
    b->GetMesh()->SetMaterial(material3);

       auto a = std::make_shared<ModelRenderer>(ModelRenderer("Testobject", "C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/charmanderSculptFinal.obj"));
       a->GetModel()->GetMeshList()[0]->SetMaterial(material3);
   // GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/vulkanscene_shadow.obj")));
  //  GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/Cerberus/Cerberus_LP.FBX")));


    //auto sound = SoundEffectManager::AddSoundEffect("../Sound/spell.ogg");
    //SoundSource source;
    //source.Play(sound);

    {

     /*   std::shared_ptr<Material> IronmMaterial = std::make_shared<Material>(Material("IronMaterial"));
        IronmMaterial->LoadAlbedoMap("../texture/pbr/rusted_iron/albedo.png");
        IronmMaterial->LoadMetallicMap("../texture/pbr/rusted_iron/metallic.png");
        IronmMaterial->LoadRoughnessMap("../texture/pbr/rusted_iron/roughness.png");
        IronmMaterial->LoadAmbientOcclusionMap("../texture/pbr/rusted_iron/ao.png");
        IronmMaterial->LoadNormalMap("../texture/pbr/rusted_iron/normal.png");
        MaterialManager::AddMaterial(IronmMaterial);

        std::shared_ptr<Material> PlasticMaterial = std::make_shared<Material>(Material("PlasticMaterial"));
        PlasticMaterial->LoadAlbedoMap("../texture/pbr/plastic/albedo.png");
        PlasticMaterial->LoadMetallicMap("../texture/pbr/plastic/metallic.png");
        PlasticMaterial->LoadRoughnessMap("../texture/pbr/plastic/roughness.png");
        PlasticMaterial->LoadAmbientOcclusionMap("../texture/pbr/plastic/ao.png");
        PlasticMaterial->LoadNormalMap("../texture/pbr/plastic/normal.png");
        MaterialManager::AddMaterial(PlasticMaterial);

        std::shared_ptr<Material> WallMaterial = std::make_shared<Material>(Material("WallMaterial"));
        WallMaterial->LoadAlbedoMap("../texture/pbr/wall/albedo.png");
        WallMaterial->LoadMetallicMap("../texture/pbr/wall/metallic.png");
        WallMaterial->LoadRoughnessMap("../texture/pbr/wall/roughness.png");
        WallMaterial->LoadAmbientOcclusionMap("../texture/pbr/wall/ao.png");
        WallMaterial->LoadNormalMap("../texture/pbr/wall/normal.png");
        MaterialManager::AddMaterial(WallMaterial);

        std::shared_ptr<Material> GoldMaterial = std::make_shared<Material>(Material("GoldMaterial"));
        GoldMaterial->LoadAlbedoMap("../texture/pbr/gold/albedo.png");
        GoldMaterial->LoadMetallicMap("../texture/pbr/gold/metallic.png");
        GoldMaterial->LoadRoughnessMap("../texture/pbr/gold/roughness.png");
        GoldMaterial->LoadAmbientOcclusionMap("../texture/pbr/gold/ao.png");
        GoldMaterial->LoadNormalMap("../texture/pbr/gold/normal.png");
        MaterialManager::AddMaterial(GoldMaterial);

        std::shared_ptr<Material> GrassMaterial = std::make_shared<Material>(Material("GrassMaterial"));
        GrassMaterial->LoadAlbedoMap("../texture/pbr/grass/albedo.png");
        GrassMaterial->LoadMetallicMap("../texture/pbr/grass/metallic.png");
        GrassMaterial->LoadRoughnessMap("../texture/pbr/grass/roughness.png");
        GrassMaterial->LoadAmbientOcclusionMap("../texture/pbr/grass/ao.png");
        GrassMaterial->LoadNormalMap("../texture/pbr/grass/normal.png");
        MaterialManager::AddMaterial(GrassMaterial);

        std::shared_ptr<GameObject3D> obj = std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/sphere.obj", glm::vec3(-6.0f, 0.0f, 0.0f)));
        auto a = obj->GetComponentByType(ComponentType::kMeshRenderer);
        auto b = static_cast<MeshRenderer*>(a.get());
        b->GetModel()->GetMeshList()[0]->SetMaterial(IronmMaterial);
        GameObjectManager::AddGameObject(obj);

        std::shared_ptr<GameObject3D> obj2 = std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/sphere.obj", glm::vec3(-3.0f, 0.0f, 0.0f)));
        auto a2 = obj2->GetComponentByType(ComponentType::kMeshRenderer);
        auto b2 = static_cast<MeshRenderer*>(a2.get());
        b2->GetModel()->GetMeshList()[0]->SetMaterial(PlasticMaterial);
        GameObjectManager::AddGameObject(obj2);

        std::shared_ptr<GameObject3D> obj3 = std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/sphere.obj", glm::vec3(0.0f, 0.0f, 0.0f)));
        auto a3 = obj3->GetComponentByType(ComponentType::kMeshRenderer);
        auto b3 = static_cast<MeshRenderer*>(a3.get());
        b3->GetModel()->GetMeshList()[0]->SetMaterial(WallMaterial);
        GameObjectManager::AddGameObject(obj3);

        std::shared_ptr<GameObject3D> obj4 = std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/sphere.obj", glm::vec3(3.0f, 0.0f, 0.0f)));
        auto a4 = obj4->GetComponentByType(ComponentType::kMeshRenderer);
        auto b4 = static_cast<MeshRenderer*>(a4.get());
        b4->GetModel()->GetMeshList()[0]->SetMaterial(GoldMaterial);
        GameObjectManager::AddGameObject(obj4);

        std::shared_ptr<GameObject3D> obj5 = std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/sphere.obj", glm::vec3(6.0f, 0.0f, 0.0f)));
        auto a5 = obj5->GetComponentByType(ComponentType::kMeshRenderer);
        auto b5 = static_cast<MeshRenderer*>(a5.get());
        b5->GetModel()->GetMeshList()[0]->SetMaterial(GrassMaterial);
        GameObjectManager::AddGameObject(obj5);*/

       //GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject5", glm::vec3(0.0f), glm::vec3(5.0f))));
    }

    auto dLight = DirectionalLightBuffer{};
    dLight.diffuse = glm::vec3(0.2f);
    dLight.specular = glm::vec3(0.5f);

    LightManager::AddDirectionalLight(dLight);

    PointLightBuffer plight = PointLightBuffer();
    plight.position = glm::vec3(-10.0f, 10.0f, 10.0f);
    plight.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    plight.specular = glm::vec3(1.0f);

    PointLightBuffer plight2 = PointLightBuffer();
    plight2.position = glm::vec3(10.0f, 10.0f, 10.0f);
    plight2.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    plight2.specular = glm::vec3(1.0f);

    PointLightBuffer plight3 = PointLightBuffer();
    plight3.position = glm::vec3(-10.0f, -10.0f, 10.0f);
    plight3.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    plight3.specular = glm::vec3(1.0f);

    PointLightBuffer plight4 = PointLightBuffer();
    plight4.position = glm::vec3(10.0f, -10.0f, 10.0f);
    plight4.diffuse = glm::vec3(300.0f, 300.0f, 300.0f);
    plight4.specular = glm::vec3(1.0f);

    LightManager::AddPointLight(plight);
    LightManager::AddPointLight(plight2);
    LightManager::AddPointLight(plight3);
    LightManager::AddPointLight(plight4);

    MeshRendererManager::Update();
    TopLevelAccelerationStructureManager::Update();
    BuildRenderers();
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
        BuildRenderers();
    }

    SceneManager::Update();

    switch (SceneManager::sceneType)
    {
        case SceneType::kSprite2D:
        {
            renderer2D.Update();
            break;
        }
        case SceneType::kBlinnPhong:
        {
            if (GraphicsDevice::IsRayTracingFeatureActive())
            {
                if (GraphicsDevice::IsRayTracerActive())
                {
                    rayTraceRenderer.Update();
                }
                else if (GraphicsDevice::IsHybridRendererActive())
                {
                    hybridRenderer.Update();
                }
                else
                {
                    blinnPhongRenderer.Update();
                }
            }
            else
            {
                blinnPhongRenderer.Update();
            }
            break;
        }
        case SceneType::kPBR:
        {
            pbrRenderer.Update();
            break;
        }
    }
}

void Scene::ImGuiUpdate()
{
 /*   if (obj5 != nullptr)
    {
        const auto lineRendererComp = obj5->GetComponentByType(ComponentType::kLineRenderer3D);
        const auto lineRenderer = static_cast<LineRenderer3D*>(lineRendererComp.get());
        auto lineMesh = static_cast<LineMesh3D*>(lineRenderer->GetModel()->GetMeshList()[0].get());

        ImGui::SliderFloat3("Line Start ", &lineMesh->LineVertexList[0].pos[0], -100.0f, 100.0f);
        ImGui::SliderFloat3("Line End ", &lineMesh->LineVertexList[1].pos[0], 0.0f, 360.0f);
        ImGui::SliderFloat4("Line Color Start ", &lineMesh->LineVertexList[0].color[0], 0.0, 1.0f);
        ImGui::SliderFloat4("Line Color End ", &lineMesh->LineVertexList[1].color[0], 0.0f, 1.0f);
    }*/

    if (pbrRenderer.GetColorPickerTexture() != nullptr)
    {
        ImGui::Image(pbrRenderer.GetColorPickerTexture()->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
 /*   if (SceneManager::sceneType == SceneType::kPBR)
    {
        ImGui::Image(pbrRenderer.depthDebugRenderPass.RenderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
    else if (SceneManager::sceneType == SceneType::kBlinnPhong)
    {
        ImGui::Image(blinnPhongRenderer.depthDebugRenderPass.RenderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }*/
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


    //int i = 0;
    //const auto objList = GameObjectManager::GetGameObjectList();
    //for (auto obj : GameObjectManager::GetGameObjectList())
    //{
    //    auto modelRenderer = obj->GetComponentByType(ComponentType::kMeshRenderer);
    //        if (modelRenderer)
    //        {
    //            if (auto obj3D = static_cast<MeshRenderer*>(modelRenderer.get()))
    //            {
    //                for (int x =0; x < obj3D->GetModel()->GetMeshList().size(); x++)
    //                {
    //                    auto meshobj = static_cast<Mesh3D*>(obj3D->GetModel()->GetMeshList()[x].get());
    //                    ImGui::SliderFloat3("Mesh position " + x, &meshobj->MeshPosition.x, -100.0f, 100.0f);
    //                    ImGui::SliderFloat3("Mesh rotation " + x, &meshobj->MeshRotation.x, 0.0f, 360.0f);
    //                    ImGui::SliderFloat3("Mesh scale " + x, &meshobj->MeshScale.x, 0.0f, 1.0f);
    //                    i++;
    //                }
    //            }
    //        }
    //}

    ImGui::Checkbox("Wireframe Mode", &VulkanRenderer::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        ImGui::Checkbox("RayTrace Mode", &GraphicsDevice::RayTracingActive);
        ImGui::Checkbox("Hybrid Mode", &GraphicsDevice::HybridRendererActive);
    }

    SceneManager::ImGuiSceneHierarchy();
    MeshRendererManager::GUIUpdate();
    LightManager::GUIUpdate();

    VulkanRenderer::ImGUILayerActive = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
}

void Scene::BuildRenderers()
{
    MeshRendererManager::Update();

    switch (SceneManager::sceneType)
    {
        case SceneType::kSprite2D:
        {
            renderer2D.BuildRenderer();
            break;
        }
        case SceneType::kBlinnPhong:
        {
            blinnPhongRenderer.BuildRenderer();
            if (GraphicsDevice::IsRayTracingFeatureActive())
            {
                rayTraceRenderer.BuildRenderer();
                hybridRenderer.BuildRenderer();
            }
            break;
        }
        case SceneType::kPBR:
        {
            pbrRenderer.BuildRenderer();
            break;
        }
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
        BuildRenderers();
        return;
    }

    switch (SceneManager::sceneType)
    {
        case SceneType::kSprite2D:
        {
           renderer2D.Draw(SceneManager::sceneProperites, CommandBufferSubmitList);
            break;
        }
        case SceneType::kBlinnPhong:
        {
       
            if (GraphicsDevice::IsRayTracingFeatureActive())
            {
                if (GraphicsDevice::IsRayTracerActive())
                {
                    rayTraceRenderer.Draw(SceneManager::sceneProperites, CommandBufferSubmitList);
                }
                else if (GraphicsDevice::IsHybridRendererActive())
                {
                    hybridRenderer.Draw(SceneManager::sceneProperites, CommandBufferSubmitList);
                }
                else
                {
                    blinnPhongRenderer.Draw(SceneManager::sceneProperites, SceneManager::cubeMapInfo, CommandBufferSubmitList);
                }
            }
            else
            {
                blinnPhongRenderer.Draw(SceneManager::sceneProperites, SceneManager::cubeMapInfo, CommandBufferSubmitList);
            }
            break;
        }
        case SceneType::kPBR:
        {
            pbrRenderer.Draw(CommandBufferSubmitList);
            break;
        }
    }
 
    InterfaceRenderPass::Draw();
    CommandBufferSubmitList.emplace_back(InterfaceRenderPass::ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

    result = VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        BuildRenderers();
        return;
    }
}

void Scene::Destroy()
{
    GameObjectManager::Destroy();

    switch (SceneManager::sceneType)
    {
        case SceneType::kSprite2D:
        {
            renderer2D.Destroy();
            break;
        }
        case SceneType::kBlinnPhong:
        {
            blinnPhongRenderer.Destroy();
            if (GraphicsDevice::IsRayTracingFeatureActive())
            {
                hybridRenderer.Destroy();
                rayTraceRenderer.Destroy();
            }
            break;
        }
        case SceneType::kPBR:
        {
            pbrRenderer.Destroy();
            break;
        }
    }
}
