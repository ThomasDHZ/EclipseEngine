#include "PBRRealTimeRenderer.h"

PBRRealTimeRenderer::PBRRealTimeRenderer()
{
}

PBRRealTimeRenderer::~PBRRealTimeRenderer()
{
}
void PBRRealTimeRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
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
		pbrBloomRenderPass.BuildRenderPass(submitList);
		blurRenderPass.BuildRenderPass(pbrBloomRenderPass.BloomMapList);
		bloomCombinePipeline.BuildRenderPass(blurRenderPass.BlurredTextureList);
	}

	//	depthDebugRenderPass.BuildRenderPass(DepthPassRenderPass.DepthTextureList[0]);
	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture, bloomCombinePipeline.BloomTexture);
	AnimationRenderer.StartUp();
}

void PBRRealTimeRenderer::Update()
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

void PBRRealTimeRenderer::ImGuiUpdate()
{
	if (SceneManager::EditorModeFlag)
	{
		if (ImGui::Button("Play Mode"))
		{
			SceneManager::EditorModeFlag = false;
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

void PBRRealTimeRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (SceneManager::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	//Depth Pass
	{
		CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
		CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
		//CommandBufferSubmitList.emplace_back(spotLightDepthPassRenderPassList[x].Draw());
	}

	//SkyBox Pass
	{
		auto reflectingMesh = MeshRendererManager::GetMeshByID(31);
		CommandBufferSubmitList.emplace_back(skyIrradianceRenderPass.Draw());
		CommandBufferSubmitList.emplace_back(skyPrefilterRenderPass.Draw());
		CommandBufferSubmitList.emplace_back(skyPBRRenderPass.Draw(reflectingMesh));
	}
	//Geometry Pass
	{
		//auto reflectingMesh = MeshRendererManager::GetMeshByID(31);
		CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw());
		CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw());
		CommandBufferSubmitList.emplace_back(ReflectionPreRenderPass.Draw());
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

	//CommandBufferSubmitList.emplace_back(depthDebugRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRRealTimeRenderer::Destroy()
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
		ReflectionPreRenderPass.Destroy();
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

	//depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
	AnimationRenderer.Destroy();
}
