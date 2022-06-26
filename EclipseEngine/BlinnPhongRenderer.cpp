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
	depthRenderPass.StartUp();
	blinnPhongRenderPass.StartUp(depthRenderPass.DepthTexture);
	depthDebugRenderPass.StartUp(depthRenderPass.DepthTexture);
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
	depthRenderPass.RebuildSwapChain();
	blinnPhongRenderPass.RebuildSwapChain(depthRenderPass.DepthTexture);
	depthDebugRenderPass.RebuildSwapChain(depthRenderPass.DepthTexture);
	frameBufferRenderPass.RebuildSwapChain(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	CommandBufferSubmitList.emplace_back(depthRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(blinnPhongRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void BlinnPhongRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	depthRenderPass.Destroy();
	blinnPhongRenderPass.Destroy();
	depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
