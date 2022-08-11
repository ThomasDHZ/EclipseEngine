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
		SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;

		brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());
		skyBoxIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyBoxPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyBoxPBRRenderPass.OneTimeDraw(skyBoxIrradianceRenderPass.IrradianceCubeMap, skyBoxPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPreRenderedMapSize());

		geoIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		geoPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		geoPBRRenderPass.OneTimeDraw(geoIrradianceRenderPass.IrradianceCubeMap, geoPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPreRenderedMapSize());

		irradianceRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());

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
		SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;

		brdfRenderPass.BuildRenderPass(SceneManager::GetPBRCubeMapSize());
		//Depth Pass
		{
			depthPassRendererPass.BuildRenderPass();
		}
		//Reflection Pass
		{
			geoIrradianceRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			geoPrefilterRenderPass.BuildRenderPass(SceneManager::CubeMap, SceneManager::GetPBRCubeMapSize());
			geoPBRRenderPass.BuildRenderPass(geoIrradianceRenderPass.IrradianceCubeMap, geoPrefilterRenderPass.PrefilterCubeMap, SceneManager::GetPBRCubeMapSize());
		}
		//Main Render Pass
		{
			irradianceRenderPass.BuildRenderPass(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPBRCubeMapSize());
			prefilterRenderPass.BuildRenderPass(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPBRCubeMapSize());
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
			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(geoPBRRenderPass.Draw());
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
		geoIrradianceRenderPass.Destroy();
		geoPrefilterRenderPass.Destroy();
		geoPBRRenderPass.Destroy();
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
