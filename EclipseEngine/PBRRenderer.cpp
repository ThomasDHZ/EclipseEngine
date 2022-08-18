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

	std::vector<std::shared_ptr<RenderedDepthTexture>> directionalDepthTextureList;
	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> cubeMapDepthTextureList;


	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(512);
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	//Depth Pass
	{
		depthPassRenderPassList.resize(LightManager::GetDirectionalLightCount());
		for (auto& directionalLight : depthPassRenderPassList)
		{
			directionalLight.BuildRenderPass(glm::vec2(512.0f, 512.0f));
			directionalDepthTextureList.emplace_back(directionalLight.DepthTexture);
		}

		depthCubeMapRenderPassList.resize(LightManager::GetPointLightCount());
		for(auto& pointLight : depthCubeMapRenderPassList)
		{
			pointLight.BuildRenderPass(glm::vec2(512.0f, 512.0f));
			cubeMapDepthTextureList.emplace_back(pointLight.DepthTexture);
		}
	}
	//Skybox Pass
	{
		skyBoxIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyBoxPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyBoxPBRRenderPass.OneTimeDraw(skyBoxIrradianceRenderPass.IrradianceCubeMap, skyBoxPrefilterRenderPass.PrefilterCubeMap, directionalDepthTextureList[0], SceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f));
	}
	//Geometry Pass
	{
		geoIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
		geoPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
		geoPBRRenderPass.OneTimeDraw(geoIrradianceRenderPass.IrradianceCubeMap, geoPrefilterRenderPass.PrefilterCubeMap, directionalDepthTextureList[0], SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
	}
	//Main Render Pass
	{
		irradianceRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());
		pbrRenderPass.BuildRenderPass(irradianceRenderPass.IrradianceCubeMap, prefilterRenderPass.PrefilterCubeMap, directionalDepthTextureList[0], cubeMapDepthTextureList);
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
		if (!UpdatePreRenderer)
		{
			//Depth Pass
			{
				for (int x = 0; x < LightManager::GetDirectionalLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(depthPassRenderPassList[x].Draw());
				}
				for (int x = 0; x < LightManager::GetPointLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(depthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
				}
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
				for (int x = 0; x < LightManager::GetDirectionalLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(depthPassRenderPassList[x].Draw());
				}
				for (int x = 0; x < LightManager::GetPointLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(depthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
				}
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
			for (int x = 0; x < LightManager::GetDirectionalLightCount(); x++)
			{
				CommandBufferSubmitList.emplace_back(depthPassRenderPassList[x].Draw());
			}
			for (int x = 0; x < LightManager::GetPointLightCount(); x++)
			{
				CommandBufferSubmitList.emplace_back(depthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
			}
		}

		//Geometry Pass
		{
			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
			CommandBufferSubmitList.emplace_back(geoPBRRenderPass.Draw(glm::vec3(3.3f, 10.0f, 1.0f)));
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
		for (auto& directionalLight : depthPassRenderPassList)
		{
			directionalLight.Destroy();
		}
		for (auto pointLight : depthCubeMapRenderPassList)
		{
			pointLight.Destroy();
		}
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
