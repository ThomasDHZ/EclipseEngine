#include "PBRRenderer.h"

PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;

	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(512);
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	//Depth Pass
	{
		depthPassRendererPass.BuildRenderPass();
	}
	//Skybox Pass
	{
		skyBoxIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
		skyBoxPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
		skyBoxPBRRenderPass.OneTimeDraw(skyBoxIrradianceRenderPass.IrradianceCubeMap, skyBoxPrefilterRenderPass.PrefilterCubeMap, depthPassRendererPass.DepthTexture, SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
	}
	//Geometry Pass
	{
		geoIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
		geoPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
		geoPBRRenderPass.OneTimeDraw(geoIrradianceRenderPass.IrradianceCubeMap, geoPrefilterRenderPass.PrefilterCubeMap, depthPassRendererPass.DepthTexture, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
	}
	//Main Render Pass
	{
		irradianceRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
		prefilterRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
		pbrRenderPass.BuildRenderPass(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap, depthPassRendererPass.DepthTexture);
	}

	depthDebugRenderPass.BuildRenderPass(depthPassRendererPass.DepthTexture);
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
		if (!UpdatePreRenderer)
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

			//Geometry Pass
			{
				CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
				CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
				CommandBufferSubmitList.emplace_back(geoPBRRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
			}
			//Main Render Pass
			{
				CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw(glm::vec3(0.0f)));
				CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw(glm::vec3(0.0f)));
				CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
			}

			UpdatePreRenderer = false;
		}
	}
	else
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(depthPassRendererPass.Draw());
		}

		//Geometry Pass
		{
			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
			CommandBufferSubmitList.emplace_back(geoPBRRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw(glm::vec3(0.0f)));
			CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw(glm::vec3(0.0f)));
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
		}
	}

	CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
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
	//Skybox Pass
	{
		skyBoxIrradianceRenderPass.Destroy();
		skyBoxPrefilterRenderPass.Destroy();
		skyBoxPBRRenderPass.Destroy();
	}
	//Geometry Pass
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
