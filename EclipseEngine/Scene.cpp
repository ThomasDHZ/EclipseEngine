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
#include "MeshRenderer.h"

std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    SceneManager::sceneType = SceneType::kPBR;

   // SceneManager::activeCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 10.5f));
    SceneManager::activeCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));
   // SceneManager::LoadScene("../Scenes/example.txt");
//
//    std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial", MaterialTypeEnum::kMaterialBlinnPhong));
//    material->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Diffuse.png");
//    material->LoadAlphaMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/Mario_Alpha.png");
//    MaterialManager::AddMaterial(material);
//
//
//    std::shared_ptr<Material> material2 = std::make_shared<Material>(Material("TestMaterial2", MaterialTypeEnum::kMaterialBlinnPhong));
//    material2->LoadDiffuseMap("C:/Users/dotha/source/repos/VulkanGraphics/texture/space-cruiser-panels2_albedo.png");
//    MaterialManager::AddMaterial(material2);
//
// /*    glm::vec2 StartPoint = glm::vec2(0.0f);
//        glm::vec2 EndPoint = glm::vec2(5.0f);
//        glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.3f);
//;     obj5 = std::make_shared<GameObject2D>(GameObject2D("Testobject5", StartPoint, EndPoint, Color));
//       GameObjectManager::AddGameObject(obj5);*/
//
//   /*    glm::vec3 StartPoint2 = glm::vec3(1.0f);
//       glm::vec3 EndPoint2 = glm::vec3(5.0f);
//       glm::vec4 Color2 = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
//       auto   obj6 = std::make_shared<LineRenderer3D>(LineRenderer3D("Testobject5", StartPoint2, EndPoint2, Color2));*/
//
//       //glm::vec2 StartPoint = glm::vec2(0.0f);
//       //glm::vec2 EndPoint = glm::vec2(5.0f);
//       //glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.3f);
//       //;   auto  obj6 = std::make_shared<LineRenderer2D>(LineRenderer2D("Testobject5", StartPoint, EndPoint, Color));
//       //GameObjectManager::AddGameObject(obj6);
//
//    //std::vector<MeshVertex> vertices = {
//    //    {{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//    //    {{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//    //    {{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//    //    {{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//    //};
//
//    // std::vector<uint32_t> indices = {
//    //    0, 1, 2, 2, 3, 0
//    //};
//     //std::shared_ptr<SpriteRenderer> obj = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject", material, glm::vec2(0.0f), 0));
//     //std::shared_ptr<SpriteRenderer> obj2 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject2", material2, glm::vec2(1.0f, 0.0f), 0));
//     //std::shared_ptr<SpriteRenderer> obj3 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject3", material, glm::vec2(2.0f, 0.0f), 0));
//     //std::shared_ptr<SpriteRenderer> obj4 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject4", material2, glm::vec2(3.0f, 0.0f), 0));
//
//    //GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject5", glm::vec3(0.0f), glm::vec3(5.0f, 5.0f, 5.0f))));
//  //  std::shared_ptr<GameObject3D> obj5 = std::make_shared<GameObject3D>(GameObject3D("Testobject6", 5, .25));
//   //  std::shared_ptr<GridRenderer3D> obj5 = std::make_shared<GridRenderer3D>(GridRenderer3D("Testobject6", 50, 1.0f));
//    //GameObjectManager::AddGameObject(obj5);
//
//    //int width = 500;
//    //int height = 500;
//    //float length = 10.0f;
//    //float radius = 0.5f;
//    //std::vector<LineVertex3D> VertexList;
//    //for (uint32_t y = 0; y < height; y++)
//    //{
//    //    for (uint32_t x = 0; x < width; x++)
//    //    {
//    //        glm::vec2 coord = { (float)x / width, (float)y / height };
//    //        coord = coord * 2.0f - 1.0f;
//
//    //        uint8_t r = (uint8_t)(coord.x * 255.0f);
//    //        uint8_t g = (uint8_t)(coord.y * 255.0f);
//
//    //        glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
//    //        glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
//
//    //        float a = glm::dot(rayDirection, rayDirection);
//    //        float b = 2.0f * glm::dot(rayOrigin, rayDirection);
//    //        float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
//
//    //        //Quadratic forumla discriminat
//    //        //b^2 - 4ac;
//
//    //        float discriminant = (b * b) - 4.0f * a * c;
//
//    //        if (discriminant >= 0.0f)
//    //        {
//    //            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(1.0f, 0.0f, 0.8f, 1.0f)));
//    //            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(1.0f, 0.0f, 0.8, 1.0f)));
//    //        }
//    //        else
//    //        {
//    //            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.0f, 0.0f, 01.0f, 0.02f)));
//    //            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.02f)));
//    //        }
//    //    }
//    //}
//    //auto line = std::make_shared<LineRenderer3D>(LineRenderer3D("Testobject5", VertexList));
//
//
////   SceneManager::LoadScene("../Scenes/example.txt");
//
//    CubeMapLayout cubeMapfiles;
//    cubeMapfiles.Left = "../texture/skybox/right.jpg";
//    cubeMapfiles.Right = "../texture/skybox/left.jpg";
//    cubeMapfiles.Top = "../texture/skybox/top.jpg";
//    cubeMapfiles.Bottom = "../texture/skybox/bottom.jpg";
//    cubeMapfiles.Front = "../texture/skybox/back.jpg";
//    cubeMapfiles.Back = "../texture/skybox/front.jpg";
//    TextureManager::LoadCubeMapTexture(cubeMapfiles);
//
    SceneManager::environmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/Buildings-Night-Vis-4K.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

//std::make_shared<ModelRenderer>(ModelRenderer("sponza", "../Models/Sponza/sponza.obj"));
//    //std::shared_ptr<Material> material3 = std::make_shared<Material>(Material("HyruleShield", MaterialTypeEnum::kMaterialPBR));
//    //material3->LoadAlbedoMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/hyruleshieldNormalDone_Material.004_BaseColor.bmp");
//    //material3->LoadMetallicMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/hyruleshieldNormalDone_Material.004_Metallic.bmp");
//    //material3->LoadNormalMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/hyruleshieldNormalDone_Material.004_Normal.bmp");
//    //material3->LoadRoughnessMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/hyruleshieldNormalDone_Material.004_Roughness.bmp");
//    //material3->LoadAmbientOcclusionMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/Material.004_ambient_occlusion.bmp");
//    //MaterialManager::SaveMaterial(material3);
//  //  material3->LoadDepthMap("C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/hyruleshieldNormalDone_Material.004_Height.bmp");
//  //  material3->MaterialBufferUpdate();
//  //  MaterialManager::LoadMaterial();
//
//
//
//    //auto b= std::make_shared<BillBoardMeshRenderer>(BillBoardMeshRenderer("Testobject2", glm::vec2(1.0f)));
//    //b->GetMesh()->SetMaterial(material3);
//
//       //auto a = std::make_shared<ModelRenderer>(ModelRenderer("Testobject", "C:/Users/dotha/source/repos/EclipseEngine/Models/Charmander/charmanderSculptFinal.obj"));
//       //a->GetModel()->GetMeshList()[0]->SetMaterial(material3);
//
//    std::shared_ptr<Material> HyruleShield = MaterialManager::LoadMaterial("../Materials/HyruleShield.txt");
//       auto a = std::make_shared<ModelRenderer>(ModelRenderer("HyruleShield", "C:/Users/dotha/source/repos/EclipseEngine/Models/Shield/Shield.obj"));
//       a->GetModel()->GetMeshList()[0]->SetMaterial(HyruleShield);
//       //SceneManager::SaveAsPrefab(a);
//
//   // GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/vulkanscene_shadow.obj")));
//  //  GameObjectManager::AddGameObject(std::make_shared<GameObject3D>(GameObject3D("Testobject", "../Models/Cerberus/Cerberus_LP.FBX")));
//
//
//    //auto sound = SoundEffectManager::AddSoundEffect("../Sound/spell.ogg");
//    //SoundSource source;
//    //source.Play(sound);
//
    {
        //std::shared_ptr<Material> IronmMaterial = MaterialManager::LoadMaterial("../Materials/IronMaterial.txt");
        //std::shared_ptr<Material> PlasticMaterial = MaterialManager::LoadMaterial("../Materials/PlasticMaterial.txt");
        //std::shared_ptr<Material> WallMaterial = MaterialManager::LoadMaterial("../Materials/WallMaterial.txt");
        //std::shared_ptr<Material> GoldMaterial = MaterialManager::LoadMaterial("../Materials/GoldMaterial.txt");
        //std::shared_ptr<Material> GrassMaterial = MaterialManager::LoadMaterial("../Materials/GrassMaterial.txt");

        //auto obj = std::make_shared<ModelRenderer>(ModelRenderer("IronSphere", "../Models/sphere.obj", glm::vec3(-6.0f, 0.0f, 0.0f)));
        //obj->GetModel()->GetMeshList()[0]->SetMaterial(IronmMaterial);
        //SceneManager::SaveAsPrefab(obj);

        //auto obj2 = std::make_shared<ModelRenderer>(ModelRenderer("PlasticSphere", "../Models/sphere.obj", glm::vec3(-3.0f, 0.0f, 0.0f)));
        //obj2->GetModel()->GetMeshList()[0]->SetMaterial(PlasticMaterial);
        //SceneManager::SaveAsPrefab(obj2);

        //auto obj3 = std::make_shared<ModelRenderer>(ModelRenderer("WallSphere", "../Models/sphere.obj"));
        //obj3->GetModel()->GetMeshList()[0]->SetMaterial(GoldMaterial);
        //SceneManager::SaveAsPrefab(obj3);

        //auto obj4 = std::make_shared<ModelRenderer>(ModelRenderer("GoldSphere", "../Models/sphere.obj", glm::vec3(3.0f, 0.0f, 0.0f)));
        //obj4->GetModel()->GetMeshList()[0]->SetMaterial(GoldMaterial);
        //MeshRendererManager::GetMeshByID(30)->SetReflectionPoint(glm::vec3(3.3f, 1.0f, 1.0f));
        //SceneManager::SaveAsPrefab(obj4);

        //auto obj5 = std::make_shared<ModelRenderer>(ModelRenderer("GrassSphere", "../Models/sphere.obj", glm::vec3(6.0f, 0.0f, 0.0f)));
        //obj5->GetModel()->GetMeshList()[0]->SetMaterial(GoldMaterial);
        //SceneManager::SaveAsPrefab(obj5);

    InstancingDataStruct instance = {};
    instance.GameObjectMatrix = glm::mat4(1.0f);
    instance.ModelMatrix = glm::mat4(1.0f);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            for (int z = 0; z < 5; z++)
            {
                InstanceMeshDataStruct instanceMeshDataStruct = {};
                instanceMeshDataStruct.InstancePosition = glm::vec3(float(x), float(y), float(z));
                instance.instanceMeshDataList.emplace_back(instanceMeshDataStruct);
            }
        }
    }

        std::vector<Vertex3D> vertices = {
        {{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
        {{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
        {{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
        {{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
    };

     std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };

        std::shared_ptr<Material> material3 = std::make_shared<Material>(Material("Grass", MaterialTypeEnum::kMaterialPBR));
    material3->LoadAlbedoMap("C:/Users/dotha/source/repos/EclipseEngine/texture/grass.png");
    MaterialManager::SaveMaterial(material3);

    auto obj5 = std::make_shared<MeshRenderer>(MeshRenderer("Grass", vertices, indices, instance, material3));
    }
//
    auto dLight = DirectionalLightBuffer{};
    dLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dLight.diffuse = glm::vec3(30.8f);
    dLight.specular = glm::vec3(1.0f);

    //auto dLight2 = DirectionalLightBuffer{};
    //dLight2.direction = glm::vec3(-1.0f, -1.0f, 0.0f);
    //dLight2.diffuse = glm::vec3(30.8f);
    //dLight2.specular = glm::vec3(1.0f);

    LightManager::AddDirectionalLight(dLight);
   // LightManager::AddDirectionalLight(dLight2);
    //LightManager::AddDirectionalLight(dLight);
    //LightManager::AddDirectionalLight(dLight);

    //PointLightBuffer plight = PointLightBuffer();
    //plight.position = glm::vec3(-118.49f, 16.47f, 39.53f);
    //plight.diffuse = glm::vec3(300.0f);
    //plight.specular = glm::vec3(1.0f);

    //PointLightBuffer plight2 = PointLightBuffer();
    //plight2.position = glm::vec3(-121.08f, 16.47f, -44.34);
    //plight2.diffuse = glm::vec3(300.0f);
    //plight2.specular = glm::vec3(1.0f);

    //PointLightBuffer plight3 = PointLightBuffer();
    //plight3.position = glm::vec3(110.98f, 16.47f, 39.53f);
    //plight3.diffuse = glm::vec3(300.0f);
    //plight3.specular = glm::vec3(1.0f);

    //PointLightBuffer plight4 = PointLightBuffer();
    //plight4.position = glm::vec3(110.98f, 16.47f, -44.34);
    //plight4.diffuse = glm::vec3(300.0f);
    //plight4.specular = glm::vec3(1.0f);

    //LightManager::AddPointLight(plight);
    //LightManager::AddPointLight(plight2);
    //LightManager::AddPointLight(plight3);
    //LightManager::AddPointLight(plight4);
//
//    SceneManager::SaveScene("../Scenes/example.txt");

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
    //if (pbrRenderer.GetColorPickerTexture() != nullptr)
    //{
    //    ImGui::Image(pbrRenderer.GetColorPickerTexture()->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    //}
    if (SceneManager::sceneType == SceneType::kPBR)
    {
        //ImGui::Image(pbrRenderer.depthDebugRenderPass.RenderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
        //ImGui::Image(pbrRenderer.depthDebugRenderPass2.RenderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
    else if (SceneManager::sceneType == SceneType::kBlinnPhong)
    {
        //ImGui::Image(blinnPhongRenderer.depthDebugRenderPass.RenderedTexture->ImGuiDescriptorSet, ImVec2(VulkanRenderer::GetSwapChainResolution().width / 5, VulkanRenderer::GetSwapChainResolution().height / 5));
    }
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

    ImGui::Checkbox("Wireframe Mode", &VulkanRenderer::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        ImGui::Checkbox("RayTrace Mode", &GraphicsDevice::RayTracingActive);
        ImGui::Checkbox("Hybrid Mode", &GraphicsDevice::HybridRendererActive);
    }

    if (SceneManager::sceneType == SceneType::kPBR)
    {
        bool OnChange = ImGui::Checkbox("PreRender Mode", &pbrRenderer.PreRenderedFlag);
        if(OnChange)
        {
            BuildRenderers();
        }
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
