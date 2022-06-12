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

	//Reflection Pass
	{
		reflectionIrradianceRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());
		reflectionPrefilterRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());

		//SceneManager::ReflectionIrradianceCubeMap = reflectionIrradianceRenderPass.IrradianceCubeMap;
		//SceneManager::ReflectionPrefilterCubeMap = reflectionPrefilterRenderPass.PrefilterCubeMap;

		//pbrReflectionRenderPass.StartUp();

	}
	////Main Render Pass
	{
		irradianceRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());
		prefilterRenderPass.StartUp(SceneManager::GetPBRCubeMapSize());

		SceneManager::IrradianceCubeMap = irradianceRenderPass.IrradianceCubeMap;
		SceneManager::PrefilterCubeMap = prefilterRenderPass.PrefilterCubeMap;

		pbrRenderPass.StartUp();
	}
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

	//Reflection Pass
	{
		reflectionIrradianceRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());
		reflectionPrefilterRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());
		//pbrReflectionRenderPass.RebuildSwapChain();

		SceneManager::ReflectionIrradianceCubeMap = reflectionIrradianceRenderPass.IrradianceCubeMap;
		SceneManager::ReflectionPrefilterCubeMap = reflectionPrefilterRenderPass.PrefilterCubeMap;
	}
	////Main Render Pass
	{
		irradianceRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());
		prefilterRenderPass.RebuildSwapChain(SceneManager::GetPBRCubeMapSize());
		pbrRenderPass.RebuildSwapChain();

		SceneManager::IrradianceCubeMap = irradianceRenderPass.IrradianceCubeMap;
		SceneManager::PrefilterCubeMap = prefilterRenderPass.PrefilterCubeMap;
	}

	frameBufferRenderPass.RebuildSwapChain(pbrRenderPass.RenderedTexture);
}

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		meshPickerRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());
	}

	//Reflection Pass
	{
		reflectionIrradianceRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(reflectionIrradianceRenderPass.GetCommandBuffer());

		reflectionPrefilterRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(reflectionPrefilterRenderPass.GetCommandBuffer());

		//pbrReflectionRenderPass.Draw();
		//CommandBufferSubmitList.emplace_back(pbrReflectionRenderPass.GetCommandBuffer());
	}
	////Main Render Pass
	{
		irradianceRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(irradianceRenderPass.GetCommandBuffer());

		prefilterRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(prefilterRenderPass.GetCommandBuffer());

		pbrRenderPass.Draw();
		CommandBufferSubmitList.emplace_back(pbrRenderPass.GetCommandBuffer());
	}

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();
	reflectionIrradianceRenderPass.Destroy();
	reflectionPrefilterRenderPass.Destroy();
	pbrReflectionRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	pbrRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
