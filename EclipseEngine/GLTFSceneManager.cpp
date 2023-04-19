#include "GLTFSceneManager.h"
SkyBoxView							               GLTFSceneManager::CubeMapInfo;
SceneProperties							           GLTFSceneManager::sceneProperites;
std::shared_ptr<Camera>							   GLTFSceneManager::ActiveCamera;
std::shared_ptr<Skybox>                            GLTFSceneManager::SkyboxMesh;
std::vector<std::shared_ptr<GLTFMaterial>>		   GLTFSceneManager::MaterialList;
std::vector<std::shared_ptr<Texture2D>>			   GLTFSceneManager::Texture2DList;
std::shared_ptr<EnvironmentTexture>                GLTFSceneManager::EnvironmentTexture = nullptr;
std::shared_ptr<RenderedColorTexture>              GLTFSceneManager::BRDFTexture = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::IrradianceMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::PrefilterMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture>            GLTFSceneManager::CubeMap = nullptr;
std::vector<std::shared_ptr<GameObject>>		   GLTFSceneManager::GameObjectList;
std::vector<std::shared_ptr<GLTFSunLight>>         GLTFSceneManager::SunLightList;
std::vector<std::shared_ptr<GLTFDirectionalLight>> GLTFSceneManager::DirectionalLightList;
std::vector<std::shared_ptr<GLTFPointLight>>       GLTFSceneManager::PointLightList;
std::vector<std::shared_ptr<GLTFSpotLight>>        GLTFSceneManager::SpotLightList;
//std::shared_ptr<Camera>					       GLTFSceneManager::activeCamera;
float                                              GLTFSceneManager::PBRCubeMapSize = 256.0f;
float									           GLTFSceneManager::PreRenderedMapSize = 256.0f;
bool											   GLTFSceneManager::WireframeModeFlag = false;

void GLTFSceneManager::UpdateBufferIndex()
{
	for (int x = 0; x < Texture2DList.size(); x++)
	{
		Texture2DList[x]->UpdateTextureBufferIndex(x);
	}

	for (int x = 0; x < MaterialList.size(); x++)
	{
		MaterialList[x]->UpdateMaterialBufferIndex(x);
	}
}

std::shared_ptr<Texture2D> GLTFSceneManager::IsTexture2DLoaded(const GLTFTextureLoader& textureLoader)
{
	uint64_t textureID = 0;
	for (auto texture : Texture2DList)
	{
		if (texture->GetFilePath() == textureLoader.TextureLoader.uri)
		{
			return texture;
		}
	}

	return nullptr;
}

std::shared_ptr<Texture2D> GLTFSceneManager::IsTexture2DLoaded(std::string name)
{
	uint64_t textureID = 0;
	for (auto texture : Texture2DList)
	{
		if (texture->GetFilePath() == name)
		{
			return texture;
		}
	}

	return nullptr;
}

std::shared_ptr<Texture2D> GLTFSceneManager::LoadTexture2D(std::shared_ptr<Texture2D> texture)
{
	std::shared_ptr<Texture2D> isTextureLoaded = IsTexture2DLoaded(texture->GetFilePath());
	if (isTextureLoaded != nullptr)
	{
		return isTextureLoaded;
	}

	Texture2DList.emplace_back(texture);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
	return texture;
}

std::shared_ptr<Texture2D> GLTFSceneManager::LoadTexture2D(GLTFTextureLoader& textureLoader)
{
	std::shared_ptr<Texture2D> isTextureLoaded = IsTexture2DLoaded(textureLoader);
	if (isTextureLoaded != nullptr)
	{
		return isTextureLoaded;
	}

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(Texture2D(textureLoader));
	Texture2DList.emplace_back(texture);
	Texture2DList.back()->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
	return texture;
}

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

void GLTFSceneManager::AddPointLight(std::shared_ptr<GLTFPointLight> pointLight)
{
	PointLightList.emplace_back(pointLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddSpotLight(std::shared_ptr<GLTFSpotLight> spotLight)
{
	SpotLightList.emplace_back(spotLight);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddSunLight(GLTFSunLightLoader& sunLight)
{
	std::shared_ptr<GLTFSunLight> light = std::make_shared<GLTFSunLight>(GLTFSunLight(sunLight.name, sunLight.position, sunLight.diffuse, sunLight.intensity));
	SunLightList.emplace_back(light);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddDirectionalLight(GLTFDirectionalLightLoader& directionalLight)
{
	std::shared_ptr<GLTFDirectionalLight> light = std::make_shared<GLTFDirectionalLight>(GLTFDirectionalLight(directionalLight.name, directionalLight.direction, directionalLight.diffuse, directionalLight.intensity));
	DirectionalLightList.emplace_back(light);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddPointLight(GLTFPointLightLoader& pointLight)
{
	std::shared_ptr<GLTFPointLight> light = std::make_shared<GLTFPointLight>(GLTFPointLight(pointLight.name, pointLight.position, pointLight.diffuse, pointLight.intensity, pointLight.radius));
	PointLightList.emplace_back(light);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::AddSpotLight(GLTFSpotLightLoader& spotLight)
{
	std::shared_ptr<GLTFSpotLight> light = std::make_shared<GLTFSpotLight>(GLTFSpotLight(spotLight.name, spotLight.position, spotLight.direction, spotLight.diffuse, spotLight.intensity));
	SpotLightList.emplace_back(light);
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFSceneManager::StartUp()
{
	SkyboxMesh = std::make_shared<Skybox>();
	SkyboxMesh->StartUp();
}

void GLTFSceneManager::Update()
{
	for (auto& light : SunLightList)
	{
		light->Update();
	}
	for (auto& light : DirectionalLightList)
	{
		light->Update();
	}
	for (auto& light : PointLightList)
	{
		light->Update();
	}
	for (auto& light : SpotLightList)
	{
		light->Update();
	}

	SkyboxMesh->Update(ActiveCamera);

	sceneProperites.CameraPos = GLTFSceneManager::ActiveCamera->GetPosition();
	sceneProperites.view = GLTFSceneManager::ActiveCamera->GetViewMatrix();
	sceneProperites.proj = GLTFSceneManager::ActiveCamera->GetProjectionMatrix();
	sceneProperites.Timer = (float)glfwGetTime();
	sceneProperites.frame++;
	if (sceneProperites.frame == UINT32_MAX)
	{
		sceneProperites.frame = 0;
	}
	sceneProperites.MaxReflectCount = 2;
}

void GLTFSceneManager::Destroy()
{
	SkyboxMesh->Destroy();
	if (EnvironmentTexture != nullptr)
	{
		EnvironmentTexture->Destroy();
	}
	if (BRDFTexture != nullptr)
	{
		BRDFTexture->Destroy();
	}
	if (IrradianceMap != nullptr)
	{
		IrradianceMap->Destroy();
	}
	if (PrefilterMap != nullptr)
	{
		PrefilterMap->Destroy();
	}
	if (CubeMap != nullptr)
	{
		CubeMap->Destroy();
	}
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

VkDescriptorImageInfo GLTFSceneManager::GetEnvironmentMapDescriptor()
{
	if (EnvironmentTexture != nullptr)
	{
		VkDescriptorImageInfo SkyboxBufferInfo;
		SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		SkyboxBufferInfo.imageView = GLTFSceneManager::EnvironmentTexture->View;
		SkyboxBufferInfo.sampler = GLTFSceneManager::EnvironmentTexture->Sampler;
		return SkyboxBufferInfo;
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