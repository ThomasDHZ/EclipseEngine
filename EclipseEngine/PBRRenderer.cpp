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
	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> pointDepthTextureList;
	std::vector<std::shared_ptr<RenderedDepthTexture>> spotDepthTextureList;

	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(512);
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	//Depth Pass
	{
		directionalLightDepthPassRenderPassList.resize(LightManager::GetDirectionalLightCount());
		for (auto& directionalLight : directionalLightDepthPassRenderPassList)
		{
			directionalLight.BuildRenderPass(glm::vec2(512.0f, 512.0f));
			directionalDepthTextureList.emplace_back(directionalLight.DepthTexture);
		}

		pointLightDepthCubeMapRenderPassList.resize(LightManager::GetPointLightCount());
		for(auto& pointLight : pointLightDepthCubeMapRenderPassList)
		{
			pointLight.BuildRenderPass(glm::vec2(512.0f, 512.0f));
			pointDepthTextureList.emplace_back(pointLight.DepthTexture);
		}

		spotLightDepthPassRenderPassList.resize(LightManager::GetSpotLightCount());
		for (auto& spotLight : spotLightDepthPassRenderPassList)
		{
			spotLight.BuildRenderPass(glm::vec2(512.0f, 512.0f));
			spotDepthTextureList.emplace_back(spotLight.DepthTexture);
		}

	}

	PBRRenderPassTextureSubmitList submitList;
	submitList.DirectionalLightTextureShadowMaps = directionalDepthTextureList;
	submitList.PointLightShadowMaps = pointDepthTextureList;
	submitList.SpotLightTextureShadowMaps = spotDepthTextureList;

	//Skybox Pass
	{
		skyBoxIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyBoxPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTexture = skyBoxIrradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = skyBoxPrefilterRenderPass.PrefilterCubeMap;

		skyBoxPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize());
	}
	//Geometry Pass
	{
		geoIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
		geoPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));

		submitList.IrradianceTexture = geoIrradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = geoPrefilterRenderPass.PrefilterCubeMap;

		geoPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize(), glm::vec3(3.3f, 10.0f, 1.0f));
	}
	//Main Render Pass
	{
		irradianceRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.OneTimeDraw(geoPBRRenderPass.ReflectionCubeMapTexture, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTexture = irradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = prefilterRenderPass.PrefilterCubeMap;

		pbrRenderPass.BuildRenderPass(submitList);
	}

	//depthDebugRenderPass.BuildRenderPass(directionalLightDepthPassRenderPassList[0].DepthTexture);
	//depthDebugRenderPass2.BuildRenderPass(directionalLightDepthPassRenderPassList[1].DepthTexture);
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
					CommandBufferSubmitList.emplace_back(directionalLightDepthPassRenderPassList[x].Draw());
				}
				for (int x = 0; x < LightManager::GetPointLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(pointLightDepthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
				}
				for (int x = 0; x < LightManager::GetSpotLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
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
					CommandBufferSubmitList.emplace_back(directionalLightDepthPassRenderPassList[x].Draw());
				}
				for (int x = 0; x < LightManager::GetPointLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(pointLightDepthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
				}
				for (int x = 0; x < LightManager::GetSpotLightCount(); x++)
				{
					CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
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
				CommandBufferSubmitList.emplace_back(directionalLightDepthPassRenderPassList[x].Draw());
			}
			for (int x = 0; x < LightManager::GetPointLightCount(); x++)
			{
				CommandBufferSubmitList.emplace_back(pointLightDepthCubeMapRenderPassList[x].Draw(LightManager::GetPointLights()[x]->GetPosition()));
			}
			for (int x = 0; x < LightManager::GetSpotLightCount(); x++)
			{
				CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
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
	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass2.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	//Depth Pass
	{
		for (auto& directionalLight : directionalLightDepthPassRenderPassList)
		{
			directionalLight.Destroy();
		}
		for (auto pointLight : pointLightDepthCubeMapRenderPassList)
		{
			pointLight.Destroy();
		}
		for (auto& spotLight : spotLightDepthPassRenderPassList)
		{
			spotLight.Destroy();
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
	 
	//depthDebugRenderPass.Destroy();
	//depthDebugRenderPass2.Destroy();
	frameBufferRenderPass.Destroy();
}
