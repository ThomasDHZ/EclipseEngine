#include "BlinnPhongRenderer.h"

BlinnPhongRenderer::BlinnPhongRenderer()
{
}

BlinnPhongRenderer::~BlinnPhongRenderer()
{
}

void BlinnPhongRenderer::StartUp()
{
	meshPickerRenderPass.StartUp();
	blinnPhongRenderPass.StartUp();
	frameBufferRenderPass.StartUp(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Update()
{
	if (VulkanRenderer::EditorModeFlag &&
		!VulkanRenderer::ImGUILayerActive &&
		Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
		const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

		MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
	}
}

void BlinnPhongRenderer::RebuildRenderers()
{
	meshPickerRenderPass.RebuildSwapChain();
	blinnPhongRenderPass.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw(sceneProperties);
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	blinnPhongRenderPass.Draw(sceneProperties, skyboxView);
	CommandBufferSubmitList.emplace_back(blinnPhongRenderPass.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void BlinnPhongRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	blinnPhongRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
