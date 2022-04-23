#include "BlinnPhongRenderer.h"

BlinnPhongRenderer::BlinnPhongRenderer()
{
}

BlinnPhongRenderer::~BlinnPhongRenderer()
{
}

void BlinnPhongRenderer::StartUp()
{
	raytraceHybridPass.StartUp();
	meshPickerRenderPass.StartUp();
	blinnPhongRenderPass.StartUp(raytraceHybridPass.RenderedShadowTexture);
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
	raytraceHybridPass.RebuildSwapChain();
	meshPickerRenderPass.RebuildSwapChain();
	blinnPhongRenderPass.RebuildSwapChain(raytraceHybridPass.RenderedShadowTexture);
	frameBufferRenderPass.RebuildSwapChain(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	meshPickerRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());

	blinnPhongRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(blinnPhongRenderPass.GetCommandBuffer());

	//raytraceHybridPass.Draw(sceneProperites);
	//CommandBufferSubmitList.emplace_back(raytraceHybridPass.RayTraceCommandBuffer);

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void BlinnPhongRenderer::Destroy()
{
	raytraceHybridPass.Destroy();
	meshPickerRenderPass.Destroy();
	blinnPhongRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
