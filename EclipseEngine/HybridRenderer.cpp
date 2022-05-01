#include "HybridRenderer.h"

HybridRenderer::HybridRenderer()
{
}

HybridRenderer::~HybridRenderer()
{
}

void HybridRenderer::StartUp()
{
	raytraceHybridPass.StartUp();
	meshPickerRenderPass.StartUp();
	GBufferRenderPass.StartUp(raytraceHybridPass.RenderedShadowTexture);
	deferredRenderPass.StartUp(GBufferRenderPass.PositionTexture,
								GBufferRenderPass.TangentTexture,
								GBufferRenderPass.BiTangentTexture,
								GBufferRenderPass.TBNormalTexture,
								GBufferRenderPass.NormalTexture,
								GBufferRenderPass.AlbedoTexture,
								GBufferRenderPass.SpecularTexture,
								GBufferRenderPass.BloomTexture,
								raytraceHybridPass.RenderedShadowTexture);
	frameBufferRenderPass.StartUp(deferredRenderPass.RenderedTexture);
}

void HybridRenderer::Update()
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

void HybridRenderer::RebuildRenderers()
{
	raytraceHybridPass.RebuildSwapChain();
	meshPickerRenderPass.RebuildSwapChain();
	GBufferRenderPass.RebuildSwapChain(raytraceHybridPass.RenderedShadowTexture);
	deferredRenderPass.RebuildSwapChain(GBufferRenderPass.PositionTexture,
										GBufferRenderPass.TangentTexture,
										GBufferRenderPass.BiTangentTexture,
										GBufferRenderPass.TBNormalTexture,
										GBufferRenderPass.NormalTexture,
										GBufferRenderPass.AlbedoTexture,
										GBufferRenderPass.SpecularTexture,
										GBufferRenderPass.BloomTexture,
										raytraceHybridPass.RenderedShadowTexture);
	frameBufferRenderPass.RebuildSwapChain(deferredRenderPass.RenderedTexture);
}

void HybridRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw(sceneProperites);
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	GBufferRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(GBufferRenderPass.GetCommandBuffer());

	deferredRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(deferredRenderPass.GetCommandBuffer());

	raytraceHybridPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(raytraceHybridPass.RayTraceCommandBuffer);

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void HybridRenderer::Destroy()
{
	raytraceHybridPass.Destroy();
	meshPickerRenderPass.Destroy();
	GBufferRenderPass.Destroy();
	deferredRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
