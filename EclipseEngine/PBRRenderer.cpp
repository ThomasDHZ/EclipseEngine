#include "PBRRenderer.h"

PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(512);

	if (PreRenderedFlag)
	{
		brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());
		reflectionIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		reflectionPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		pbrReflectionRenderPass.OneTimeDraw(reflectionIrradianceRenderPass.IrradianceCubeMap, reflectionPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPreRenderedMapSize());
		irradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());

		//Depth Pass
		{
			depthPassRendererPass.BuildRenderPass();
		}

		//Main Render Pass
		{
			pbrRenderPass.BuildRenderPass(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap);
		}
	}
	else
	{
		brdfRenderPass.BuildRenderPass(SceneManager::GetPBRCubeMapSize());
		//Depth Pass
		{
			depthPassRendererPass.BuildRenderPass();
		}
		//Reflection Pass
		{
			reflectionIrradianceRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			reflectionPrefilterRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			pbrReflectionRenderPass.BuildRenderPass(reflectionIrradianceRenderPass.IrradianceCubeMap, reflectionPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPBRCubeMapSize());
		}
		//Main Render Pass
		{
			irradianceRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			prefilterRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			pbrRenderPass.BuildRenderPass(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap);
		}
	}

	//depthDebugRenderPass.BuildRenderPass(depthPassRendererPass.DepthTexture);
	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture);
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

	if (MeshRendererManager::GetSelectedMesh())
	{
		MeshRendererManager::GetSelectedMesh()->SetSelectedMesh(true);
	}
}

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	if (PreRenderedFlag)
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(depthPassRendererPass.Draw());
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
		}
	}
	else
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(depthPassRendererPass.Draw());
		}
		//Reflection Pass
		{
			CommandBufferSubmitList.emplace_back(reflectionIrradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(reflectionPrefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(pbrReflectionRenderPass.Draw());
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
		}
	}

	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	//Depth Pass
	{
		depthPassRendererPass.Destroy();
	}
	//Reflection Pass
	{
		reflectionIrradianceRenderPass.Destroy();
		reflectionPrefilterRenderPass.Destroy();
		pbrReflectionRenderPass.Destroy();
	}
	//Main Render Pass
	{
		irradianceRenderPass.Destroy();
		prefilterRenderPass.Destroy();
		pbrRenderPass.Destroy();
	}
	 
//	depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
