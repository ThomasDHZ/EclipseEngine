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
	environmentToCubeRenderPass.StartUp(2048.0f);
	brdfRenderPass.StartUp(2048.0f);
	irradianceRenderPass.StartUp(environmentToCubeRenderPass.RenderedCubeMap, 2048.0f);
	prefilterRenderPass.StartUp(environmentToCubeRenderPass.RenderedCubeMap, 2048.0f);
	pbrRenderPass.StartUp(brdfRenderPass.BRDFMap, irradianceRenderPass.RenderedCubeMap, prefilterRenderPass.RenderedCubeMap, environmentToCubeRenderPass.RenderedCubeMap);
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
	environmentToCubeRenderPass.RebuildSwapChain(2048.0f);
	brdfRenderPass.RebuildSwapChain(2048.0f);
	irradianceRenderPass.RebuildSwapChain(environmentToCubeRenderPass.RenderedCubeMap, 2048.0f);
	prefilterRenderPass.RebuildSwapChain(environmentToCubeRenderPass.RenderedCubeMap, 2048.0f);
	pbrRenderPass.RebuildSwapChain(brdfRenderPass.BRDFMap, irradianceRenderPass.RenderedCubeMap, prefilterRenderPass.RenderedCubeMap, environmentToCubeRenderPass.RenderedCubeMap);
	frameBufferRenderPass.RebuildSwapChain(pbrRenderPass.RenderedTexture);
}

void PBRRenderer::Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw(sceneProperties);
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	environmentToCubeRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(environmentToCubeRenderPass.GetCommandBuffer());

	brdfRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(brdfRenderPass.GetCommandBuffer());

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
