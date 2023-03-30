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
#include "ReadableTexture.h"
#include "ParticalSystemRenderer.h"
#include "GLTF_Temp_Model.h"

std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    SceneManager::sceneType = SceneType::kPBR;
    GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));
   
    MeshRendererManager::Update();
    //TopLevelAccelerationStructureManager::Update();
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
    pbrRenderer.Update();
}

void Scene::ImGuiUpdate()
{
    SceneManager::bloomsettings.blurScale;
    pbrRenderer.ImGuiUpdate();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Wireframe Mode", &VulkanRenderer::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        ImGui::Checkbox("RayTrace Mode", &SceneManager::RayTracingActive);
        ImGui::Checkbox("Hybrid Mode", &SceneManager::HybridRendererActive);
    }


    SceneManager::ImGuiSceneHierarchy();
    MeshRendererManager::GUIUpdate();
    VulkanRenderer::ImGUILayerActive = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
}

void Scene::BuildRenderers()
{
    MeshRendererManager::Update();
    pbrRenderer.BuildRenderer();
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

    pbrRenderer.Draw(CommandBufferSubmitList);
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
    pbrRenderer.Destroy();
}
