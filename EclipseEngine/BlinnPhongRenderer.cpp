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
	if (!VulkanRenderer::ImGUILayerActive &&
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

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	meshPickerRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());

	blinnPhongRenderPass.Draw(sceneProperites);
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
