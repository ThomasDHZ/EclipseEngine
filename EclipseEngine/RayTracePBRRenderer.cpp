#include "RayTracePBRRenderer.h"

RayTracePBRRenderer::RayTracePBRRenderer() : RenderPass()
{
}

RayTracePBRRenderer::~RayTracePBRRenderer()
{
}

void RayTracePBRRenderer::BuildRenderer()
{
    GLTFSceneManager::AddDirectionalLight(std::make_shared<DirectionalLight>(DirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));

    rayTraceRenderPass.StartUp();
    frameBufferRenderPass.BuildRenderPass(rayTraceRenderPass.RayTracedTexture, rayTraceRenderPass.RayTracedTexture);
}

void RayTracePBRRenderer::Update()
{
    rayTraceRenderPass.Update();
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
    rayTraceRenderPass.Destroy();
    frameBufferRenderPass.Destroy();
}

