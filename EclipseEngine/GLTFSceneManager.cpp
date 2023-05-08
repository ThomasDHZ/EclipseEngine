#include "GLTFSceneManager.h"
SkyBoxView							               GLTFSceneManager::CubeMapInfo;
SceneProperties							           GLTFSceneManager::sceneProperites;
std::shared_ptr<Camera>							   GLTFSceneManager::ActiveCamera;
std::shared_ptr<Skybox>                            GLTFSceneManager::SkyboxMesh;
std::vector<std::shared_ptr<Material>>			   GLTFSceneManager::MaterialList;
std::vector<std::shared_ptr<Texture>>			   GLTFSceneManager::TextureList;
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

	for (int x = 0; x < GameObjectList.size(); x++)
	{
		for (int y = 0; y < GameObjectList[x]->GetGameObjectRenderer()->GetMeshList().size(); y++)
		{
			GameObjectList[x]->GetGameObjectRenderer()->GetMeshList()[y]->UpdateMeshBufferIndex(x);
		}
	}
	for (int x = 0; x < TextureList.size(); x++)
	{
		TextureList[x]->UpdateTextureBufferIndex(x);
	}
	for (int x = 0; x < MaterialList.size(); x++)
	{
		MaterialList[x]->UpdateMaterialBufferIndex(x);
	}
}

std::shared_ptr<Texture> GLTFSceneManager::IsTexture2DLoaded(const GLTFTextureLoader& textureLoader)
{
	uint64_t textureID = 0;
	for (auto texture : TextureList)
	{
		if (texture->GetFilePath() == textureLoader.TextureLoader.uri)
		{
			return texture;
		}
	}

	return nullptr;
}

std::shared_ptr<Texture> GLTFSceneManager::IsTexture2DLoaded(std::string name)
{
	uint64_t textureID = 0;
	for (auto texture : TextureList)
	{
		if (texture->GetFilePath() == name)
		{
			return texture;
		}
	}

	return nullptr;
}

std::shared_ptr<Texture> GLTFSceneManager::LoadTexture2D(std::shared_ptr<Texture> texture)
{
	std::shared_ptr<Texture> isTextureLoaded = IsTexture2DLoaded(texture->GetFilePath());
	if (isTextureLoaded != nullptr)
	{
		return isTextureLoaded;
	}

	TextureList.emplace_back(texture);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
	return texture;
}

std::shared_ptr<Texture> GLTFSceneManager::LoadTexture2D(GLTFTextureLoader& textureLoader)
{
	std::shared_ptr<Texture> isTextureLoaded = IsTexture2DLoaded(textureLoader);
	if (isTextureLoaded != nullptr)
	{
		return isTextureLoaded;
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture2D>(Texture2D(textureLoader));
	TextureList.emplace_back(texture);
	TextureList.back()->UpdateImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
	return texture;
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath, instanceData);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath, instanceData);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath, instanceData);
}

void GLTFSceneManager::AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kModelRenderer)));
	GameObjectList.back()->LoadRenderObject<Vertex3D>(FilePath, instanceData);
}

void GLTFSceneManager::AddLineGameObject3D( std::string Name, std::vector<LineVertex3D>& lineVertexList)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kLineRenderer3D)));
	GameObjectList.back()->LoadLineRenderObject(lineVertexList);
}

void GLTFSceneManager::AddLineGameObject3D( std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kLineRenderer3D)));
	GameObjectList.back()->LoadLineRenderObject(StartPoint, EndPoint);
}

void GLTFSceneManager::AddLineGameObject3D( std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kLineRenderer3D)));
	GameObjectList.back()->LoadLineRenderObject(StartPoint, EndPoint, Color);
}

void GLTFSceneManager::AddLineGameObject3D( std::string Name, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ)
{
	GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, GameObjectRenderType::kLineRenderer3D)));
	GameObjectList.back()->LoadLineRenderObject(GridSizeX, GridSizeY, GridSizeZ, GridSpacingX, GridSpacingY, GridSpacingZ);
}

void GLTFSceneManager::AddMaterial(const std::shared_ptr<Material> material)
{
	MaterialList.emplace_back(material);
	UpdateBufferIndex();
	VulkanRenderer::UpdateRendererFlag = true;
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
	if (VulkanRenderer::UpdateRendererFlag)
	{
		UpdateBufferIndex();
	}
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
	for (auto& obj : GameObjectList)
	{
		obj->Update(VulkanRenderer::GetFrameTimeDurationMilliseconds());
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
	for (auto& obj : GameObjectList)
	{
		obj->Destroy();
	}
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

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetVertexPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo>	MeshPropertiesBuffer;
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		MeshPropertiesBuffer.emplace_back(nullBuffer);

		return MeshPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetIndexPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo>	MeshPropertiesBuffer;
	VkDescriptorBufferInfo nullBuffer;
	nullBuffer.buffer = VK_NULL_HANDLE;
	nullBuffer.offset = 0;
	nullBuffer.range = VK_WHOLE_SIZE;
	MeshPropertiesBuffer.emplace_back(nullBuffer);

	return MeshPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetGameObjectPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo>	MeshPropertiesBuffer;
	if (GameObjectList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		MeshPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& gameObject : GameObjectList)
		{
			VkDescriptorBufferInfo MeshProperitesBufferInfo = {};
			MeshProperitesBufferInfo.buffer = gameObject->GetGameObjectPropertiesBuffer()[0].buffer;
			MeshProperitesBufferInfo.offset = 0;
			MeshProperitesBufferInfo.range = VK_WHOLE_SIZE;
			MeshPropertiesBuffer.emplace_back(MeshProperitesBufferInfo);
		}
	}

	return MeshPropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetGameObjectTransformBuffer()
{
	std::vector<VkDescriptorBufferInfo>	TransformPropertiesBuffer;
	if (GameObjectList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		TransformPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& gameObject : GameObjectList)
		{
			VkDescriptorBufferInfo TransformBufferInfo = {};
			TransformBufferInfo.buffer = gameObject->GetGameObjectTransformMatrixBuffer()[0].buffer;
			TransformBufferInfo.offset = 0;
			TransformBufferInfo.range = VK_WHOLE_SIZE;
			TransformPropertiesBuffer.emplace_back(TransformBufferInfo);
		}
	}

	return TransformPropertiesBuffer;
}

std::vector<VkDescriptorImageInfo> GLTFSceneManager::GetTexturePropertiesBuffer()
{
	std::vector<VkDescriptorImageInfo>	TexturePropertiesBuffer;
	if (TextureList.size() == 0)
	{
		TexturePropertiesBuffer.emplace_back(VulkanRenderer::GetNullDescriptor());
	}
	else
	{
		for (auto& texture : TextureList)
		{
			VkDescriptorImageInfo albedoTextureDescriptor{};
			albedoTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			albedoTextureDescriptor.imageView = texture->GetView();
			albedoTextureDescriptor.sampler = texture->GetSampler();
			TexturePropertiesBuffer.emplace_back(albedoTextureDescriptor);
		}
	}

	return TexturePropertiesBuffer;
}

std::vector<VkDescriptorBufferInfo> GLTFSceneManager::GetMaterialPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo>	MaterialPropertiesBuffer;
	if (SunLightList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		MaterialPropertiesBuffer.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& material : MaterialList)
		{
			material->GetMaterialPropertiesBuffer(MaterialPropertiesBuffer);
		}
	}

	return MaterialPropertiesBuffer;
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