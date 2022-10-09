#include "PBREditorRenderer.h"

PBREditorRenderer::PBREditorRenderer()
{
}

PBREditorRenderer::~PBREditorRenderer()
{
}

void PBREditorRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(4096.0f/4);
	cubeToEnvironmentRenderPass.BuildRenderPass(glm::vec2(4096.0f, 4096.0f/2), SceneManager::CubeMap);
	cubeToEnvironmentRenderPass.environmentMap->BakeEnvironmentMapTexture("EnvironmentMap.hdr");
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());



	///Bake pipleineSetUp
		//Depth Pass
	{
		bakeDepthPassRenderPass.OneTimeDraw(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
		bakeDepthCubeMapRenderPass.OneTimeDraw(LightManager::GetPointLights(), glm::vec2(512.0f));
		//spotLightDepthPassRenderPassList.BuildRenderPass(glm::vec2(512.0f, 512.0f));
	}


	bakesubmitList.DirectionalLightTextureShadowMaps = bakeDepthPassRenderPass.DepthTextureList;
	bakesubmitList.PointLightShadowMaps = bakeDepthCubeMapRenderPass.DepthCubeMapTextureList;
	//submitList.SpotLightTextureShadowMaps = spotDepthTextureList;

	//SkyBox Pass
	{
		bakeskyIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, 256.0f);
		bakeskyPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, 256.0f);

		bakesubmitList.IrradianceTexture = bakeskyIrradianceRenderPass.IrradianceCubeMap;
		bakesubmitList.PrefilterTexture = bakeskyPrefilterRenderPass.PrefilterCubeMap;

		bakeskyPBRRenderPass.OneTimeDraw(bakesubmitList, SceneManager::GetPreRenderedMapSize());
	}
	//Geometry Pass
	{
		bakegeoIrradianceRenderPass.OneTimeDraw(bakeskyPBRRenderPass.RenderedTexture, 256.0f);
		bakegeoPrefilterRenderPass.OneTimeDraw(bakeskyPBRRenderPass.RenderedTexture, 256.0f);

		bakesubmitList.IrradianceTexture = bakegeoIrradianceRenderPass.IrradianceCubeMap;
		bakesubmitList.PrefilterTexture = bakegeoPrefilterRenderPass.PrefilterCubeMap;

		bakegeoPBRRenderPass.OneTimeDraw(bakesubmitList, 256.0f);
	}

	///






	//Depth Pass
	{
		DepthPassRenderPass.BuildRenderPass(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
		DepthCubeMapRenderPass.BuildRenderPass(LightManager::GetPointLights(), glm::vec2(512.0f));
		//spotLightDepthPassRenderPassList.BuildRenderPass(glm::vec2(512.0f, 512.0f));
	}

	PBRRenderPassTextureSubmitList submitList;
	submitList.DirectionalLightTextureShadowMaps = DepthPassRenderPass.DepthTextureList;
	submitList.PointLightShadowMaps = DepthCubeMapRenderPass.DepthCubeMapTextureList;
	//submitList.SpotLightTextureShadowMaps = spotDepthTextureList;

	//SkyBox Pass
	{
		skyIrradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
		skyPrefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTexture = skyIrradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = skyPrefilterRenderPass.PrefilterCubeMap;

		skyPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize());
	}
	//Geometry Pass
	{
		geoIrradianceRenderPass.OneTimeDraw(skyPBRRenderPass.RenderedTexture, SceneManager::GetPreRenderedMapSize());
		geoPrefilterRenderPass.OneTimeDraw(skyPBRRenderPass.RenderedTexture, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTexture = geoIrradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = geoPrefilterRenderPass.PrefilterCubeMap;

		geoPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize());
	}
	//Main Render Pass
	{
		irradianceRenderPass.BuildRenderPass(geoPBRRenderPass.RenderedTexture, SceneManager::GetPreRenderedMapSize());
		prefilterRenderPass.BuildRenderPass(geoPBRRenderPass.RenderedTexture, SceneManager::GetPreRenderedMapSize());

		submitList.IrradianceTexture = irradianceRenderPass.IrradianceCubeMap;
		submitList.PrefilterTexture = prefilterRenderPass.PrefilterCubeMap;

		pbrRenderPass.BuildRenderPass(submitList);
		pbrBloomRenderPass.BuildRenderPass(submitList);
		blurRenderPass.BuildRenderPass(pbrBloomRenderPass.BloomMapList);
		bloomCombinePipeline.BuildRenderPass(blurRenderPass.BlurredTextureList);
	}

//	depthDebugRenderPass.BuildRenderPass(DepthPassRenderPass.DepthTextureList[0]);
	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture, bloomCombinePipeline.BloomTexture);
}

void PBREditorRenderer::Update()
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

void PBREditorRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}


	if (!UpdateRenderer)
	{
		//Depth Pass
		{
			CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
			//CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
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
			//CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
		}

		//Geometry Pass
		{
			auto reflectingMesh = MeshRendererManager::GetMeshByID(31);
			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(geoPBRRenderPass.Draw(reflectingMesh));
		}
		//Main Render Pass
		{
			CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(pbrBloomRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(blurRenderPass.Draw());
			CommandBufferSubmitList.emplace_back(bloomCombinePipeline.Draw());
		}

		UpdateRenderer = false;
	}

	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBREditorRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	//Depth Pass
	{
		DepthPassRenderPass.Destroy();
		DepthCubeMapRenderPass.Destroy();
		//spotLightDepthPassRenderPassList.Destroy();
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
		geoPBRRenderPass.Destroy();
	}
	//Main Render Pass
	{
		irradianceRenderPass.Destroy();
		prefilterRenderPass.Destroy();
		pbrRenderPass.Destroy();
		pbrBloomRenderPass.Destroy();
		blurRenderPass.Destroy();
		bloomCombinePipeline.Destroy();
	}

	bakeDepthPassRenderPass.Destroy();
	bakeDepthCubeMapRenderPass.Destroy();
	bakeskyIrradianceRenderPass.Destroy();
	bakeskyPrefilterRenderPass.Destroy();
	bakeskyPBRRenderPass.Destroy();
	bakegeoIrradianceRenderPass.Destroy();
	bakegeoPrefilterRenderPass.Destroy();
	bakegeoPBRRenderPass.Destroy();

	 
	//depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}

std::shared_ptr<BakedTexture> PBREditorRenderer::BakeTextures(const char* FileName)
{

	BakeReflectionRenderPass.BakeReflectionMaps(bakesubmitList, 256.0f);
	

	return std::shared_ptr<BakedTexture>();
}