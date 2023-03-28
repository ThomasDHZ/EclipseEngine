#include "GLTFSceneManager.h"
SkyBoxView							               GLTFSceneManager::CubeMapInfo;
SceneProperties							           GLTFSceneManager::sceneProperites;
std::shared_ptr<Camera>							   GLTFSceneManager::ActiveCamera;
std::shared_ptr<Skybox>                            GLTFSceneManager::skyboxMesh;
std::shared_ptr<EnvironmentTexture>                GLTFSceneManager::EnvironmentTexture = nullptr;
std::shared_ptr<RenderedColorTexture>              GLTFSceneManager::BRDFTexture = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::IrradianceMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::PrefilterMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::CubeMap = nullptr;
std::vector<std::shared_ptr<GLTFSunLight>>         GLTFSceneManager::SunLightList;
std::vector<std::shared_ptr<GLTFDirectionalLight>> GLTFSceneManager::DirectionalLightList;
std::vector<std::shared_ptr<GLTFPointLight>>       GLTFSceneManager::PointLightList;
std::vector<std::shared_ptr<GLTFSpotLight>>        GLTFSceneManager::SpotLightList;
//std::shared_ptr<Camera>					       GLTFSceneManager::activeCamera;
float                                              GLTFSceneManager::PBRCubeMapSize = 256.0f;
float									           GLTFSceneManager::PreRenderedMapSize = 256.0f;


void GLTFSceneManager::AddSunLight(std::shared_ptr<GLTFSunLight> sunLight)
{
	SunLightList.emplace_back(sunLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddDirectionalLight(std::shared_ptr<GLTFDirectionalLight> directionalLight)
{
	DirectionalLightList.emplace_back(directionalLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddPointlLight(std::shared_ptr<GLTFPointLight> pointLight)
{
	PointLightList.emplace_back(pointLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddSpotLight(std::shared_ptr<GLTFSpotLight> spotLight)
{
	SpotLightList.emplace_back(spotLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

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

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetSunLightPropertiesBuffer()
{
	sceneProperites.SunLightCount = SunLightList.size();

	std::vector<VkDescriptorBufferInfo>	SunLightPropertiesBuffer;
	if (SunLightList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		SunLightPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& light : SunLightList)
		{
			light->GetLightPropertiesBuffer(SunLightPropertiesBuffer);
		}
	}

	return SunLightPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetDirectionalLightPropertiesBuffer()
{
	sceneProperites.DirectionalLightCount = DirectionalLightList.size();

	std::vector<VkDescriptorBufferInfo>	DirectionalLightPropertiesBuffer;
	if (DirectionalLightList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		DirectionalLightPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& light : DirectionalLightList)
		{
			light->GetLightPropertiesBuffer(DirectionalLightPropertiesBuffer);
		}
	}

	return DirectionalLightPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetPointLightPropertiesBuffer()
{
	sceneProperites.PointLightCount = PointLightList.size();

	std::vector<VkDescriptorBufferInfo>	PointLightPropertiesBuffer;
	if (PointLightList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		PointLightPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& light : PointLightList)
		{
			light->GetLightPropertiesBuffer(PointLightPropertiesBuffer);
		}
	}

	return PointLightPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetSpotLightPropertiesBuffer()
{
	sceneProperites.SpotLightCount = SpotLightList.size();

	std::vector<VkDescriptorBufferInfo>	SpotLightPropertiesBuffer;
	if (SpotLightList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		SpotLightPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& light : SpotLightList)
		{
			light->GetLightPropertiesBuffer(SpotLightPropertiesBuffer);
		}
	}

	return SpotLightPropertiesBuffer;
}