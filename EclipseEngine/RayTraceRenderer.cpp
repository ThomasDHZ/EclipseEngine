#include "RayTraceRenderer.h"

RayTraceRenderer::RayTraceRenderer() : RenderPass()
{
}

RayTraceRenderer::~RayTraceRenderer()
{
}

void RayTraceRenderer::BuildRenderer()
{
    meshPickerRenderPass.BuildRenderPass();
    rayTraceRenderPass.BuildRenderPass();
    FrameBufferRenderer.BuildRenderPass(rayTraceRenderPass.RayTracedTexture);
}

void RayTraceRenderer::Update()
{
    if (!VulkanRenderer::ImGUILayerActive &&
        Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
        const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

        MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
    }
}

void RayTraceRenderer::GUIUpdate()
{
}

void RayTraceRenderer::Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
    CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(rayTraceRenderPass.Draw());
    CommandBufferSubmitList.emplace_back(FrameBufferRenderer.Draw());
}

void RayTraceRenderer::Destroy()
{
    meshPickerRenderPass.Destroy();
    rayTraceRenderPass.Destroy();
    FrameBufferRenderer.Destroy();
}

