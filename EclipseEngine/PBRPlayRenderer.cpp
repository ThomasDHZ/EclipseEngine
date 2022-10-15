#include "PBRPlayRenderer.h"
#include "PBREditorRenderer.h"

PBRPlayRenderer::PBRPlayRenderer()
{
}

PBRPlayRenderer::~PBRPlayRenderer()
{
}

void PBRPlayRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	//Depth Pass
	{
		DepthPassRenderPass.OneTimeDraw(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
		DepthCubeMapRenderPass.OneTimeDraw(LightManager::GetPointLights(), glm::vec2(512.0f));
	}

	PBRRenderPassTextureSubmitList submitList;
	submitList.DirectionalLightTextureShadowMaps = DepthPassRenderPass.DepthTextureList;
	submitList.PointLightShadowMaps = DepthCubeMapRenderPass.DepthCubeMapTextureList;

	irradianceRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());
	prefilterRenderPass.OneTimeDraw(SceneManager::CubeMap, SceneManager::GetPreRenderedMapSize());

	submitList.IrradianceTexture = irradianceRenderPass.IrradianceCubeMap;
	submitList.PrefilterTexture = prefilterRenderPass.PrefilterCubeMap;

	pbrRenderPass.BuildRenderPass(submitList);
	pbrBloomRenderPass.BuildRenderPass(submitList);
	blurRenderPass.BuildRenderPass(pbrBloomRenderPass.BloomMapList);
	bloomCombinePipeline.BuildRenderPass(blurRenderPass.BlurredTextureList);

	//	depthDebugRenderPass.BuildRenderPass(DepthPassRenderPass.DepthTextureList[0]);
	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture, bloomCombinePipeline.BloomTexture);
}

void PBRPlayRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRPlayRenderer::Destroy()
{
	meshPickerRenderPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	//Main Render Pass
	{
		irradianceRenderPass.Destroy();
		prefilterRenderPass.Destroy();
		pbrRenderPass.Destroy();
	}

	//depthDebugRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
