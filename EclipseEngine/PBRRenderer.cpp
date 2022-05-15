#include "PBRRenderer.h"

PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::StartUp()
{
	meshPickerRenderPass.StartUp();
	environmentToCubeRenderPass.StartUp(512.0f);
	brdfRenderPass.StartUp(512.0f);
	irradianceRenderPass.StartUp(512.0f);
	prefilterRenderPass.StartUp(512.0f);
	pbrRenderPass.StartUp();
	frameBufferRenderPass.StartUp(pbrRenderPass.RenderedTexture);
}

void PBRRenderer::Update()
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

void PBRRenderer::RebuildRenderers()
{
	meshPickerRenderPass.RebuildSwapChain();
	environmentToCubeRenderPass.RebuildSwapChain(512.0f);
	brdfRenderPass.RebuildSwapChain(512.0f);
	irradianceRenderPass.RebuildSwapChain(512.0f);
	prefilterRenderPass.RebuildSwapChain(512.0f);
	pbrRenderPass.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(pbrRenderPass.RenderedTexture);
}

void PBRRenderer::Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw(sceneProperties);
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	irradianceRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(irradianceRenderPass.GetCommandBuffer());

	prefilterRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(prefilterRenderPass.GetCommandBuffer());

	pbrRenderPass.Draw(sceneProperties, skyboxView);
	CommandBufferSubmitList.emplace_back(pbrRenderPass.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	pbrRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
