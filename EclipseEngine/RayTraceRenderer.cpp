#include "RayTraceRenderer.h"

std::string RayTraceRenderer::BaseShaderFilePath = "../Shaders/";

RayTraceRenderer::RayTraceRenderer() : RenderPass()
{
}

RayTraceRenderer::~RayTraceRenderer()
{
}

void RayTraceRenderer::BuildRenderer()
{
   // meshPickerRenderPass.BuildRenderPass();
    environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
    rayTraceRenderPass.StartUp();
    FrameBufferRenderer.BuildRenderPass(rayTraceRenderPass.RayTracedTexture, rayTraceRenderPass.RayTracedTexture);
    AnimationRenderer.StartUp();
}

void RayTraceRenderer::Update()
{
    if (!VulkanRenderer::ImGUILayerActive &&
        Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
      //  const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

       // MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
    }
}

void RayTraceRenderer::ImGuiUpdate()
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

void RayTraceRenderer::Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
    //CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(rayTraceRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(FrameBufferRenderer.Draw());
    AnimationRenderer.StartUp();
}

void RayTraceRenderer::Destroy()
{
   // meshPickerRenderPass.Destroy();
    environmentToCubeRenderPass.Destroy();
    rayTraceRenderPass.Destroy();
    FrameBufferRenderer.Destroy();
}

