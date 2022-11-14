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
	if (SceneManager::CubeMap == nullptr)
	{
		environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	}
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	//Depth Pass
	{
		DepthPassRenderPass.BuildRenderPass(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
		DepthCubeMapRenderPass.BuildRenderPass(LightManager::GetPointLights(), glm::vec2(512.0f));
	}

	PBRRenderPassTextureSubmitList submitList;
	submitList.DirectionalLightTextureShadowMaps = DepthPassRenderPass.DepthTextureList;
	submitList.PointLightShadowMaps = DepthCubeMapRenderPass.DepthCubeMapTextureList;

	//SkyBox Pass
	{
		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
		skyIrradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
		skyPrefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTextureList = skyIrradianceRenderPass.IrradianceCubeMapList;
		submitList.PrefilterTextureList = skyPrefilterRenderPass.PrefilterCubeMapList;

		skyPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize());
	}
	//Geometry Pass
	{
		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { skyPBRRenderPass.RenderedTexture };
		geoIrradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
		geoPrefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTextureList = geoIrradianceRenderPass.IrradianceCubeMapList;
		submitList.PrefilterTextureList = geoPrefilterRenderPass.PrefilterCubeMapList;

		ReflectionPreRenderPass.PreRenderPass(submitList, SceneManager::GetPreRenderedMapSize());
	}
	//Main Render Pass
	{
		irradianceRenderPass.OneTimeDraw(ReflectionPreRenderPass.ReflectionCubeMapList, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.OneTimeDraw(ReflectionPreRenderPass.ReflectionCubeMapList, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTextureList = irradianceRenderPass.IrradianceCubeMapList;
		submitList.PrefilterTextureList = prefilterRenderPass.PrefilterCubeMapList;

		pbrRenderPass.BuildRenderPass(submitList);
		//pbrBloomRenderPass.BuildRenderPass(submitList);
		//blurRenderPass.BuildRenderPass(pbrBloomRenderPass.BloomMapList);
		//bloomCombinePipeline.BuildRenderPass(blurRenderPass.BlurredTextureList);
	}

//	depthDebugRenderPass.BuildRenderPass(DepthPassRenderPass.DepthTextureList[0]);
	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture, pbrRenderPass.RenderedTexture);
	AnimationRenderer.StartUp();
}

void PBRRenderer::Update()
{
	if (SceneManager::EditorModeFlag)
	{
		if (SceneManager::EditorModeFlag &&
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
}

void PBRRenderer::ImGuiUpdate()
{
	if (SceneManager::EditorModeFlag)
	{
		if (ImGui::Button("Play Mode"))
		{
			SceneManager::EditorModeFlag = false;
		}
		if (ImGui::Button("Update Renderer"))
		{
			UpdateRenderer = true;
		}
		if (ImGui::Button("Bake"))
		{
			BakeTextures("TestBake.bmp");
		}
	}
	else
	{
		if (ImGui::Button("Editor Mode"))
		{
			SceneManager::EditorModeFlag = true;
		}
	}
}

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (SceneManager::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}


	if (!UpdateRenderer)
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
			//CommandBufferSubmitList.emplace_back(pbrBloomRenderPass.Draw());
			//CommandBufferSubmitList.emplace_back(blurRenderPass.Draw());
			//CommandBufferSubmitList.emplace_back(bloomCombinePipeline.Draw());
		}
	}
	else
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
		}

		//Geometry Pass
		{
			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(ReflectionPreRenderPass.Draw());
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
		}

		UpdateRenderer = false;
	}

	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
	AnimationRenderer.Compute();
}

void PBRRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	//Depth Pass
	{
		DepthPassRenderPass.Destroy();
		DepthCubeMapRenderPass.Destroy();
	}
	//Sky Pass
	{
		skyIrradianceRenderPass.Destroy();
		skyPrefilterRenderPass.Destroy();
		skyPBRRenderPass.Destroy();
	}
	//Geometry Pass
	{
		geoIrradianceRenderPass.Destroy();
		geoPrefilterRenderPass.Destroy();
		ReflectionPreRenderPass.Destroy();
	}
	//Main Render Pass
	{
		irradianceRenderPass.Destroy();
		prefilterRenderPass.Destroy();
		pbrRenderPass.Destroy();
		//pbrBloomRenderPass.Destroy();
		//blurRenderPass.Destroy();
		//bloomCombinePipeline.Destroy();
	}
	 
	//depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
	AnimationRenderer.Destroy();
}

void PBRRenderer::BakeTextures(const char* FileName)
{
	DepthRenderPass bakeDepthPassRenderPass;
	DepthCubeMapRenderer bakeDepthCubeMapRenderPass;
	IrradianceRenderPass bakeskyIrradianceRenderPass;
	PrefilterRenderPass bakeskyPrefilterRenderPass;
	PBRReflectionRenderPass bakeskyPBRRenderPass;
	IrradianceRenderPass bakegeoIrradianceRenderPass;
	PrefilterRenderPass bakegeoPrefilterRenderPass;
	PBRReflectionRenderPass bakegeoPBRRenderPass;
	PBRReflectionPreRenderPass BakeReflectionRenderPass;

	//Depth Pass
	{
		bakeDepthPassRenderPass.OneTimeDraw(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
		bakeDepthCubeMapRenderPass.OneTimeDraw(LightManager::GetPointLights(), glm::vec2(512.0f));
	}

	PBRRenderPassTextureSubmitList bakesubmitList;
	bakesubmitList.DirectionalLightTextureShadowMaps = bakeDepthPassRenderPass.DepthTextureList;
	bakesubmitList.PointLightShadowMaps = bakeDepthCubeMapRenderPass.DepthCubeMapTextureList;

	//SkyBox Pass
	{
		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
		bakeskyIrradianceRenderPass.OneTimeDraw(cubemap, 256.0f);
		bakeskyPrefilterRenderPass.OneTimeDraw(cubemap, 256.0f);

		bakesubmitList.IrradianceTextureList = bakeskyIrradianceRenderPass.IrradianceCubeMapList;
		bakesubmitList.PrefilterTextureList = bakeskyPrefilterRenderPass.PrefilterCubeMapList;

		bakeskyPBRRenderPass.OneTimeDraw(bakesubmitList, SceneManager::GetPreRenderedMapSize());
	}
	//Geometry Pass
	{
		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { bakeskyPBRRenderPass.RenderedTexture };
		bakegeoIrradianceRenderPass.OneTimeDraw(cubemap, 256.0f);
		bakegeoPrefilterRenderPass.OneTimeDraw(cubemap, 256.0f);

		bakesubmitList.IrradianceTextureList = bakegeoIrradianceRenderPass.IrradianceCubeMapList;
		bakesubmitList.PrefilterTextureList = bakegeoPrefilterRenderPass.PrefilterCubeMapList;

		bakegeoPBRRenderPass.OneTimeDraw(bakesubmitList, 256.0f);
	}

	BakeReflectionRenderPass.BakeReflectionMaps(bakesubmitList, 256.0f, 8192/4);
	
	bakeDepthPassRenderPass.Destroy();
	bakeDepthCubeMapRenderPass.Destroy();
	bakeskyIrradianceRenderPass.Destroy();
	bakeskyPrefilterRenderPass.Destroy();
	bakeskyPBRRenderPass.Destroy();
	bakegeoIrradianceRenderPass.Destroy();
	bakegeoPrefilterRenderPass.Destroy();
	bakegeoPBRRenderPass.Destroy();
}