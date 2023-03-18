#include "GLTFSceneManager.h"
ConstSkyBoxView							GLTFSceneManager::CubeMapInfo;
SceneProperties							GLTFSceneManager::sceneProperites;
std::shared_ptr<SkyboxMesh>             GLTFSceneManager::skyboxMesh;
std::shared_ptr<EnvironmentTexture>     GLTFSceneManager::EnvironmentTexture = nullptr;
std::shared_ptr<RenderedColorTexture>   GLTFSceneManager::BRDFTexture = nullptr;
std::shared_ptr<RenderedCubeMapTexture> GLTFSceneManager::IrradianceMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> GLTFSceneManager::PrefilterMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> GLTFSceneManager::CubeMap = nullptr;
//std::shared_ptr<Camera>					GLTFSceneManager::activeCamera;
float                                   GLTFSceneManager::PBRCubeMapSize = 256.0f;
float									GLTFSceneManager::PreRenderedMapSize = 256.0f;


//void GLTFSceneManager::AddSunLight(GLTFSunLight sunLight)
//{
//	SunLightList.emplace_back(sunLight);
//}
//
//void GLTFSceneManager::AddDirectionalLight(GLTFDirectionalLight directionalLight)
//{
//	DirectionalLightList.emplace_back(directionalLight);
//}
//
//void GLTFSceneManager::AddPointlLight(GLTFPointLight pointLight)
//{
//	PointLightList.emplace_back(pointLight);
//}
//
//void GLTFSceneManager::AddSpotLight(GLTFSpotLight spotLight)
//{
//	SpotLightList.emplace_back(spotLight);
//}

void GLTFSceneManager::Update()
{
}

VkDescriptorImageInfo GLTFSceneManager::GetBRDFMapDescriptor()
{
	if (BRDFTexture != nullptr)
	{
		VkDescriptorImageInfo BRDFMapDescriptor{};
		BRDFMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		BRDFMapDescriptor.imageView = BRDFTexture->GetView();
		BRDFMapDescriptor.sampler = BRDFTexture->GetSampler();
		return BRDFMapDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFSceneManager::GetIrradianceMapDescriptor()
{
	if (IrradianceMap != nullptr)
	{
		VkDescriptorImageInfo IrradianceMapDescriptor{};
		IrradianceMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		IrradianceMapDescriptor.imageView = IrradianceMap->GetView();
		IrradianceMapDescriptor.sampler = IrradianceMap->GetSampler();
		return IrradianceMapDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFSceneManager::GetPrefilterMapDescriptor()
{
	if (PrefilterMap != nullptr)
	{
		VkDescriptorImageInfo PrefilterMapDescriptor{};
		PrefilterMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		PrefilterMapDescriptor.imageView = PrefilterMap->GetView();
		PrefilterMapDescriptor.sampler = PrefilterMap->GetSampler();
		return PrefilterMapDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

VkDescriptorImageInfo GLTFSceneManager::GetCubeMapDescriptor()
{
	if (CubeMap != nullptr)
	{
		VkDescriptorImageInfo CubeMapDescriptor{};
		CubeMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		CubeMapDescriptor.imageView = CubeMap->GetView();
		CubeMapDescriptor.sampler = CubeMap->GetSampler();
		return CubeMapDescriptor;
	}

	return VulkanRenderer::GetNullDescriptor();
}

//void GLTFSceneManager::UpdateSunLightPropertiesBuffer()
//{
//	sceneProperites.SunLightCount = SunLightList.size();
//	if (SunLightList.size() == 0)
//	{
//		VkDescriptorBufferInfo nullBuffer;
//		nullBuffer.buffer = VK_NULL_HANDLE;
//		nullBuffer.offset = 0;
//		nullBuffer.range = VK_WHOLE_SIZE;
//		SunLightPropertiesBuffer.emplace_back(nullBuffer);
//	}
//	else
//	{
//		for (auto& light : SunLightList)
//		{
//			light->GetLightPropertiesBuffer(SunLightPropertiesBuffer);
//		}
//	}
//}
//
//void GLTFSceneManager::UpdateDirectionalLightPropertiesBuffer()
//{
//	sceneProperites.DirectionalLightCount = DirectionalLightList.size();
//	if (DirectionalLightList.size() == 0)
//	{
//		VkDescriptorBufferInfo nullBuffer;
//		nullBuffer.buffer = VK_NULL_HANDLE;
//		nullBuffer.offset = 0;
//		nullBuffer.range = VK_WHOLE_SIZE;
//		DirectionalLightPropertiesBuffer.emplace_back(nullBuffer);
//	}
//	else
//	{
//		for (auto& light : DirectionalLightList)
//		{
//			light->GetLightPropertiesBuffer(DirectionalLightPropertiesBuffer);
//		}
//	}
//}
//
//void GLTFSceneManager::UpdatePointLightPropertiesBuffer()
//{
//	sceneProperites.PointLightCount = PointLightList.size();
//	if (PointLightList.size() == 0)
//	{
//		VkDescriptorBufferInfo nullBuffer;
//		nullBuffer.buffer = VK_NULL_HANDLE;
//		nullBuffer.offset = 0;
//		nullBuffer.range = VK_WHOLE_SIZE;
//		PointLightPropertiesBuffer.emplace_back(nullBuffer);
//	}
//	else
//	{
//		for (auto& light : PointLightList)
//		{
//			light->GetLightPropertiesBuffer(PointLightPropertiesBuffer);
//		}
//	}
//}
//
//void GLTFSceneManager::UpdateSpotLightPropertiesBuffer()
//{
//	sceneProperites.SpotLightCount = SpotLightList.size();
//	if (SpotLightList.size() == 0)
//	{
//		VkDescriptorBufferInfo nullBuffer;
//		nullBuffer.buffer = VK_NULL_HANDLE;
//		nullBuffer.offset = 0;
//		nullBuffer.range = VK_WHOLE_SIZE;
//		SpotLightPropertiesBuffer.emplace_back(nullBuffer);
//	}
//	else
//	{
//		for (auto& light : SpotLightList)
//		{
//			light->GetLightPropertiesBuffer(SpotLightPropertiesBuffer);
//		}
//	}
//}