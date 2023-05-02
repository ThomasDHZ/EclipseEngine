#include "RayTracePBRRenderer.h"

RayTracePBRRenderer::RayTracePBRRenderer() : RenderPass()
{
}

RayTracePBRRenderer::~RayTracePBRRenderer()
{
}

void RayTracePBRRenderer::BuildRenderer()
{
    GLTFSceneManager::StartUp();
    GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));
    GLTFSceneManager::AddDirectionalLight(std::make_shared<GLTFDirectionalLight>(GLTFDirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));

    GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/newport_loft.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);
    environmentToCubeRenderPass.OneTimeDraw(4096.0f / 4);

    rayTraceRenderPass.StartUp();
    frameBufferRenderPass.BuildRenderPass(rayTraceRenderPass.RayTracedTexture, rayTraceRenderPass.RayTracedTexture);
}

void RayTracePBRRenderer::Update()
{
    GLTFSceneManager::Update();
    //if (!VulkanRenderer::ImGUILayerActive &&
    //    Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    //{
    //    const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
    //    const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

    //  //  MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
    //}

}

void RayTracePBRRenderer::ImGuiUpdate()
{
    if (SceneManager::EditorModeFlag)
    {
        if (ImGui::Button("Play Mode"))
        {
            SceneManager::EditorModeFlag = false;
        }
    }
    else
    {
        if (ImGui::Button("Editor Mode"))
        {
            SceneManager::EditorModeFlag = true;
        }
    }
}

void RayTracePBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
    CommandBufferSubmitList.emplace_back(rayTraceRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
   // AnimationRenderer.Compute();
}

void RayTracePBRRenderer::Destroy()
{
    GLTFSceneManager::Destroy();
    environmentToCubeRenderPass.Destroy();
    rayTraceRenderPass.Destroy();
    frameBufferRenderPass.Destroy();
}

