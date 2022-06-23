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
	environmentToCubeRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());
	brdfRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());
	//Depth Pass
	{
		depthPassRendererPass.StartUp();
	}
	//Reflection Pass
	//{
	//	reflectionIrradianceRenderPass.StartUp(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
	//	reflectionPrefilterRenderPass.StartUp(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
	//	pbrReflectionRenderPass.StartUp(reflectionIrradianceRenderPass.IrradianceCubeMap, reflectionPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPBRCubeMapSize());

	//}
	////Main Render Pass
	{
		irradianceRenderPass.StartUp(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		prefilterRenderPass.StartUp(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		pbrRenderPass.StartUp(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap);
	}

	depthDebugRenderPass.StartUp(depthPassRendererPass.DepthTexture);
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
	environmentToCubeRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());
	brdfRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());

	//Depth Pass
	{
		depthPassRendererPass.RebuildSwapChain();
	}
	//Reflection Pass
	/*{
		reflectionIrradianceRenderPass.RebuildSwapChain(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		reflectionPrefilterRenderPass.RebuildSwapChain(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		pbrReflectionRenderPass.RebuildSwapChain(reflectionIrradianceRenderPass.IrradianceCubeMap, reflectionPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPBRCubeMapSize());
	}*/
	////Main Render Pass
	{
		irradianceRenderPass.RebuildSwapChain(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		prefilterRenderPass.RebuildSwapChain(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
		pbrRenderPass.RebuildSwapChain(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap);
	}

	depthDebugRenderPass.RebuildSwapChain(depthPassRendererPass.DepthTexture);
	frameBufferRenderPass.RebuildSwapChain(pbrRenderPass.RenderedTexture);
}

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	//Depth Pass
	{
		depthPassRendererPass.Draw();
		CommandBufferSubmitList.emplace_back(depthPassRendererPass.GetCommandBuffer());
	}
	//Reflection Pass
	//{
	//	reflectionIrradianceRenderPass.Draw();
	//	CommandBufferSubmitList.emplace_back(reflectionIrradianceRenderPass.GetCommandBuffer());

	//	reflectionPrefilterRenderPass.Draw();
	//	CommandBufferSubmitList.emplace_back(reflectionPrefilterRenderPass.GetCommandBuffer());

	//	pbrReflectionRenderPass.Draw();
	//	CommandBufferSubmitList.emplace_back(pbrReflectionRenderPass.GetCommandBuffer());
	//}
	////Main Render Pass
	{
		irradianceRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(irradianceRenderPass.GetCommandBuffer());

		prefilterRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(prefilterRenderPass.GetCommandBuffer());

		pbrRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(pbrRenderPass.GetCommandBuffer());
	}

	depthDebugRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(depthDebugRenderPass.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();
	depthPassRendererPass.Destroy();
	reflectionIrradianceRenderPass.Destroy();
	reflectionPrefilterRenderPass.Destroy();
	pbrReflectionRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	pbrRenderPass.Destroy();
	depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
