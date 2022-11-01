#include "RayTracePBRRenderer.h"

std::string RayTracePBRRenderer::BaseShaderFilePath = "../Shaders/";

RayTracePBRRenderer::RayTracePBRRenderer() : RenderPass()
{
}

RayTracePBRRenderer::~RayTracePBRRenderer()
{
}

void RayTracePBRRenderer::BuildRenderer()
{
   // meshPickerRenderPass.BuildRenderPass();
    environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
    rayTracePBRRenderPass.StartUp();
    FrameBufferRenderer.BuildRenderPass(rayTracePBRRenderPass.RayTracedTexture, rayTracePBRRenderPass.RayTracedTexture);
}

void RayTracePBRRenderer::Update()
{
    if (!VulkanRenderer::ImGUILayerActive &&
        Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
      //  const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

     //   MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
    }
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

void RayTracePBRRenderer::Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
  //  CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(rayTracePBRRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(FrameBufferRenderer.Draw());
}

void RayTracePBRRenderer::Destroy()
{
   // meshPickerRenderPass.Destroy();
    environmentToCubeRenderPass.Destroy();
    rayTracePBRRenderPass.Destroy();
    FrameBufferRenderer.Destroy();
}

