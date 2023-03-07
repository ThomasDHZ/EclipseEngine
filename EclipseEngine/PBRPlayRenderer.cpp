//#include "PBRPlayRenderer.h"
//#include "PBREditorRenderer.h"
//
//
//PBRPlayRenderer::PBRPlayRenderer()
//{
//}
//
//PBRPlayRenderer::~PBRPlayRenderer()
//{
//}
//
//void PBRPlayRenderer::BuildCubeMaps()
//{
//	std::vector<TextureAtlus> textureAtlasList;
//	for (int x = 0; x < 6; x++)
//	{
//		std::string filepath = "../texture/TestReflectionBakeLayer";
//		filepath.append(std::to_string(x));
//		filepath.append(".bmp");
//
//		textureAtlasList.emplace_back(TextureAtlus(filepath, glm::ivec2(256)));
//	}
//
//	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubeMapTextureList{};
//	for (int x = 0; x < textureAtlasList[0].GetTextureWidthCellCount() * textureAtlasList[0].GetTextureHeightCellCount(); x++)
//	{
//		cubeMapTextureList.emplace_back(std::make_shared<RenderedCubeMapTexture>(RenderedCubeMapTexture(textureAtlasList[0].GetTextureCellSize(), VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT)));
//	}
//
//	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();
//	for (auto& reflectionCubeMap : cubeMapTextureList)
//	{
//		reflectionCubeMap->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
//		reflectionCubeMap->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//	}
//	for (int layer = 0; layer < 6; layer++)
//	{
//		textureAtlasList[layer].UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
//	}
//	for (int y = 0; y < textureAtlasList[0].GetTextureHeightCellCount(); y++)
//	{
//		for (int x = 0; x < textureAtlasList[0].GetTextureWidthCellCount(); x++)
//		{
//			const uint32_t index = x + (y * textureAtlasList[0].GetTextureWidthCellCount());
//			for (int layer = 0; layer < 6; layer++)
//			{
//				VkImageCopy copyImage{};
//
//				copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//				copyImage.srcSubresource.layerCount = 1;
//
//				copyImage.srcOffset.x = x * textureAtlasList[0].GetTextureCellSize().x;
//				copyImage.srcOffset.y = y * textureAtlasList[0].GetTextureCellSize().y;
//				copyImage.srcOffset.z = 0;
//
//				copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//				copyImage.dstSubresource.layerCount = 1;
//				copyImage.dstSubresource.baseArrayLayer = layer;
//
//				copyImage.extent.width = cubeMapTextureList[index]->GetWidth();
//				copyImage.extent.height = cubeMapTextureList[index]->GetHeight();
//				copyImage.extent.depth = 1;
//
//				vkCmdCopyImage(commandBuffer, textureAtlasList[layer].Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, cubeMapTextureList[index]->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
//			}
//		}
//
//		for (int layer = 0; layer < 6; layer++)
//		{
//			textureAtlasList[layer].UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
//		}
//		VulkanRenderer::EndSingleTimeCommands(commandBuffer);
//	}
//	for (auto textureAtlas : textureAtlasList)
//	{
//		textureAtlas.Destroy();
//	}
//
//	irradianceRenderPass.OneTimeDraw(cubeMapTextureList, SceneManager::GetPreRenderedMapSize());
//	prefilterRenderPass.OneTimeDraw(cubeMapTextureList, SceneManager::GetPreRenderedMapSize());
//
//	for (auto cubeMap : cubeMapTextureList)
//	{
//		cubeMap->Destroy();
//	}
//}
//
//
//void PBRPlayRenderer::BuildRenderer()
//{
//	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
//	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
//	brdfRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());
//
//	//Depth Pass
//	{
//		DepthPassRenderPass.BuildRenderPass(LightManager::GetDirectionalLights(), glm::vec2(512.0f));
//		DepthCubeMapRenderPass.BuildRenderPass(LightManager::GetPointLights(), glm::vec2(512.0f));
//	}
//
//	PBRRenderPassTextureSubmitList submitList;
//	submitList.DirectionalLightTextureShadowMaps = DepthPassRenderPass.DepthTextureList;
//	submitList.PointLightShadowMaps = DepthCubeMapRenderPass.DepthCubeMapTextureList;
//
//	//SkyBox Pass
//	{
//		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
//		skyIrradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
//		skyPrefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
//
//		submitList.IrradianceTextureList = skyIrradianceRenderPass.IrradianceCubeMapList;
//		submitList.PrefilterTextureList = skyPrefilterRenderPass.PrefilterCubeMapList;
//
//		skyPBRRenderPass.OneTimeDraw(submitList, SceneManager::GetPreRenderedMapSize());
//	}
//	//Geometry Pass
//	{
//		std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { skyPBRRenderPass.RenderedTexture };
//		geoIrradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
//		geoPrefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
//
//		submitList.IrradianceTextureList = geoIrradianceRenderPass.IrradianceCubeMapList;
//		submitList.PrefilterTextureList = geoPrefilterRenderPass.PrefilterCubeMapList;
//
//		ReflectionPreRenderPass.PreRenderPass(submitList, SceneManager::GetPreRenderedMapSize());
//	}
//	//Main Render Pass
//	{
//		irradianceRenderPass.OneTimeDraw(ReflectionPreRenderPass.ReflectionCubeMapList, SceneManager::GetPreRenderedMapSize());
//		prefilterRenderPass.OneTimeDraw(ReflectionPreRenderPass.ReflectionCubeMapList, SceneManager::GetPreRenderedMapSize());
//
//		submitList.IrradianceTextureList = irradianceRenderPass.IrradianceCubeMapList;
//		submitList.PrefilterTextureList = prefilterRenderPass.PrefilterCubeMapList;
//
//		pbrRenderPass.BuildRenderPass(submitList);
//		pbrBloomRenderPass.BuildRenderPass(submitList);
//		blurRenderPass.BuildRenderPass(pbrBloomRenderPass.BloomMapList);
//		bloomCombinePipeline.BuildRenderPass(blurRenderPass.BlurredTextureList);
//	}
//
//	//	depthDebugRenderPass.BuildRenderPass(DepthPassRenderPass.DepthTextureList[0]);
//	frameBufferRenderPass.BuildRenderPass(pbrRenderPass.RenderedTexture, bloomCombinePipeline.BloomTexture);
//}
//
//void PBRPlayRenderer::Update()
//{
//
//}
//
//void PBRPlayRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
//{
//	if (!UpdateRenderer)
//	{
//		//Depth Pass
//		{
//			CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
//		}
//		//Main Render Pass
//		{
//			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
//			//CommandBufferSubmitList.emplace_back(pbrBloomRenderPass.Draw());
//			//CommandBufferSubmitList.emplace_back(blurRenderPass.Draw());
//			//CommandBufferSubmitList.emplace_back(bloomCombinePipeline.Draw());
//		}
//	}
//	else
//	{
//		//Depth Pass
//		{
//			CommandBufferSubmitList.emplace_back(DepthPassRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(DepthCubeMapRenderPass.Draw(LightManager::GetPointLights()));
//		}
//
//		//Geometry Pass
//		{
//			CommandBufferSubmitList.emplace_back(geoIrradianceRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(geoPrefilterRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(ReflectionPreRenderPass.Draw());
//		}
//		//Main Render Pass
//		{
//			CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
//			CommandBufferSubmitList.emplace_back(pbrRenderPass.Draw());
//		}
//
//		UpdateRenderer = false;
//	}
//	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
//}
//
//void PBRPlayRenderer::Destroy()
//{
//	environmentToCubeRenderPass.Destroy();
//	brdfRenderPass.Destroy();
//	irradianceRenderPass.Destroy();
//	prefilterRenderPass.Destroy();
//	pbrRenderPass.Destroy();
//	frameBufferRenderPass.Destroy();
//}
