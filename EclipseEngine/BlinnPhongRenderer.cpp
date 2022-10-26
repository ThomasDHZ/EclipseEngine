#include "BlinnPhongRenderer.h"

BlinnPhongRenderer::BlinnPhongRenderer()
{
}

BlinnPhongRenderer::~BlinnPhongRenderer()
{
}

void BlinnPhongRenderer::BuildRenderer()
{
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	//depthRenderPass.BuildRenderPass();
	blinnPhongRenderPass.BuildRenderPass(SceneManager::CubeMap, meshPickerRenderPass.depthTexture);
	//depthDebugRenderPass.BuildRenderPass(depthRenderPass.DepthTexture);
	frameBufferRenderPass.BuildRenderPass(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Update()
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

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (SceneManager::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	//CommandBufferSubmitList.emplace_back(depthRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(blinnPhongRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void BlinnPhongRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	//depthRenderPass.Destroy();
	blinnPhongRenderPass.Destroy();
	//depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
