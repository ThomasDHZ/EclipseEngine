#include "HybridRenderer.h"

HybridRenderer::HybridRenderer()
{
}

HybridRenderer::~HybridRenderer()
{
}

void HybridRenderer::BuildRenderer()
{
	raytraceHybridPass.BuildRenderPass();
	meshPickerRenderPass.BuildRenderPass();
	GBufferRenderPass.BuildRenderPass(raytraceHybridPass.RenderedShadowTexture);
	deferredRenderPass.BuildRenderPass(GBufferRenderPass.PositionTexture,
								GBufferRenderPass.TangentTexture,
								GBufferRenderPass.BiTangentTexture,
								GBufferRenderPass.TBNormalTexture,
								GBufferRenderPass.NormalTexture,
								GBufferRenderPass.AlbedoTexture,
								GBufferRenderPass.SpecularTexture,
								GBufferRenderPass.BloomTexture,
								raytraceHybridPass.RenderedShadowTexture);
	frameBufferRenderPass.BuildRenderPass(deferredRenderPass.RenderedTexture);
}

void HybridRenderer::Update()
{
	if (SceneManager::EditorModeFlag &&
		!VulkanRenderer::ImGUILayerActive &&
		Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
		const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

		MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
	}
}

void HybridRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (SceneManager::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	CommandBufferSubmitList.emplace_back(GBufferRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(deferredRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(raytraceHybridPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void HybridRenderer::Destroy()
{
	raytraceHybridPass.Destroy();
	meshPickerRenderPass.Destroy();
	GBufferRenderPass.Destroy();
	deferredRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
