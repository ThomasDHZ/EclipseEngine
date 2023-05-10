#include "Scene.h"
#include "EnvironmentTexture.h"
#include "MusicPlayer.h"
#include "Mesh2D.h"
#include "LineMesh2D.h"
#include "LineRenderer2D.h"
#include "GridRenderer2D.h"
#include "GridRenderer3D.h"
#include "BillBoardMeshRenderer.h"
#include "ReadableTexture.h"
#include "ParticalSystemRenderer.h"
#include "Model.h"

//std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    SceneManager::sceneType = SceneType::kPBR;
   
    //MeshRendererManager::Update();
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
    //spriteRenderer.Update();
    //rayTraceRenderer.Update();
}

void Scene::ImGuiUpdate()
{
    SceneManager::bloomsettings.blurScale;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Wireframe Mode", &GLTFSceneManager::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        ImGui::Checkbox("RayTrace Mode", &SceneManager::RayTracingActive);
        ImGui::Checkbox("Hybrid Mode", &SceneManager::HybridRendererActive);
    }

    pbrRenderer.ImGuiUpdate();
   // spriteRenderer.ImGuiUpdate();
    //rayTraceRenderer.ImGuiUpdate();

    SceneManager::ImGuiSceneHierarchy();
   // MeshRendererManager::GUIUpdate();
    VulkanRenderer::ImGUILayerActive = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
}

void Scene::BuildRenderers()
{
    //MeshRendererManager::Update();
    pbrRenderer.BuildRenderer();
    //spriteRenderer.BuildRenderer();
    //rayTraceRenderer.BuildRenderer();
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
    //spriteRenderer.Draw(CommandBufferSubmitList);
    //rayTraceRenderer.Draw(CommandBufferSubmitList);
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
   // GameObjectManager::Destroy();
    //rayTraceRenderer.Destroy();
    pbrRenderer.Destroy();
    //spriteRenderer.Destroy();
}
