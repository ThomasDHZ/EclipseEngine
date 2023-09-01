#include "Model.h"
#include "SceneManager.h"
#include "EngineMath.h"
#include "LineMesh3D.h"
#include "SpriteMesh2D.h"
#include "SpriteMesh3D.h"

uint64_t Model::ModelIDCounter = 0;

Model::Model()
{
}

Model::~Model()
{
}

void Model::GenerateID()
{
	ModelIDCounter++;
	ModelID = ModelIDCounter;
}

void Model::RTXModelStartUp()
{
	uint32_t PrimitiveCount = 1;
	VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst = {};

	VulkanBuffer InstanceBuffer = VulkanBuffer(nullptr, sizeof(VkAccelerationStructureInstanceKHR), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstanceBuffer.GetBuffer());

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
	AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
	AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

	VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo = {};
	AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
	AccelerationStructureBuildGeometryInfo.geometryCount = 1;
	AccelerationStructureBuildGeometryInfo.pGeometries = &AccelerationStructureGeometry;

	VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo{};
	accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	VulkanRenderer::vkGetAccelerationStructureBuildSizesKHR(VulkanRenderer::GetDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &AccelerationStructureBuildGeometryInfo, &PrimitiveCount, &accelerationStructureBuildSizesInfo);

	TopLevelAccelerationStructure.CreateAccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, accelerationStructureBuildSizesInfo);

	scratchBuffer = VulkanBuffer(nullptr, accelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	scratchBuffer.SetBufferAddress(VulkanRenderer::GetBufferDeviceAddress(scratchBuffer.GetBuffer()));

	InstanceBuffer.DestroyBuffer();
}

void Model::LoadMaterials(std::vector<GLTFMaterialLoader>& materialLoader)
{
	for (auto& loader : materialLoader)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(Material(loader.MaterialName));
		material->Albedo = loader.Albedo;
		material->Metallic = loader.Metallic;
		material->Roughness = loader.Roughness;
		material->AmbientOcclusion = loader.AmbientOcclusion;
		material->Emission = loader.Emission;
		material->Alpha = loader.Alpha;

		if (loader.AlbedoMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->AlbedoMap = GLTFSceneManager::LoadTexture2D(loader.AlbedoMap);
		}

		if (loader.MetallicRoughnessMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->MetallicRoughnessMap = GLTFSceneManager::LoadTexture2D(loader.MetallicRoughnessMap);
		}

		if (loader.AmbientOcclusionMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(loader.AmbientOcclusionMap);
		}

		if (loader.NormalMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->NormalMap = GLTFSceneManager::LoadTexture2D(loader.NormalMap);
		}

		if (loader.DepthMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->DepthMap = GLTFSceneManager::LoadTexture2D(loader.DepthMap);
		}

		if (loader.AlphaMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->AlphaMap = GLTFSceneManager::LoadTexture2D(loader.AlphaMap);
		}

		if (loader.EmissionMap.TextureType != TextureTypeEnum::kUndefinedTexture)
		{
			material->EmissionMap = GLTFSceneManager::LoadTexture2D(loader.EmissionMap);
		}

		material->UpdateBuffer();
		MaterialList.emplace_back(material);
		GLTFSceneManager::AddMaterial(material);
	}
}
void Model::LoadLevelMesh2D(const std::string& levelName, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	MaterialList = materialList;

	std::shared_ptr<LevelMesh> mesh = std::make_shared<LevelMesh>(LevelMesh(levelName, materialList, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadSpriteMesh2D(const std::string& spriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	MaterialList.emplace_back(material);

	std::shared_ptr<SpriteMesh2D> mesh = std::make_shared<SpriteMesh2D>(SpriteMesh2D(spriteName, material, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadSpriteMesh3D(const std::string& spriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	MaterialList.emplace_back(material);

	std::shared_ptr<SpriteMesh3D> mesh = std::make_shared<SpriteMesh3D>(SpriteMesh3D(spriteName, material, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadSquareMesh2D(const std::string& LineName, float Size, const glm::vec3& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<SquareMesh2D> mesh = std::make_shared<SquareMesh2D>(SquareMesh2D(LineName, Size, Color, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadSquareMesh2D(const std::string& LineName, float Size, const glm::vec4& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<SquareMesh2D> mesh = std::make_shared<SquareMesh2D>(SquareMesh2D(LineName, Size, Color, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadLineMesh2D(const std::string& LineName, std::vector<LineVertex2D>& vertices, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh2D> mesh = std::make_shared<LineMesh2D>(LineMesh2D(LineName, vertices, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh2D(const std::string& LineName, const glm::vec2& StartPoint, const glm::vec2& EndPoint, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh2D> mesh = std::make_shared<LineMesh2D>(LineMesh2D(LineName, StartPoint, EndPoint, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh2D(const std::string& LineName, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec3& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh2D> mesh = std::make_shared<LineMesh2D>(LineMesh2D(LineName, StartPoint, EndPoint, Color, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh2D(const std::string& LineName, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh2D> mesh = std::make_shared<LineMesh2D>(LineMesh2D(LineName, StartPoint, EndPoint, Color, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh2D(const std::string& LineName, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh2D> mesh = std::make_shared<LineMesh2D>(LineMesh2D(LineName, StartPoint, EndPoint, StartColor, EndColor, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadGridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<GridMesh2D> mesh = std::make_shared<GridMesh2D>(GridMesh2D(GridName, GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::LoadLineMesh3D(const std::string& LineName, std::vector<LineVertex3D>& vertices, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(LineName, vertices, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(LineName, StartPoint, EndPoint, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(LineName, StartPoint, EndPoint, Color,  GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(LineName, StartPoint, EndPoint, Color, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(LineName, StartPoint, EndPoint, StartColor, EndColor, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}
void Model::LoadLineMesh3D(const std::string& GridName, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = glm::mat4(1.0f);

	std::shared_ptr<LineMesh3D> mesh = std::make_shared<LineMesh3D>(LineMesh3D(GridName, GridSizeX, GridSizeY, GridSizeZ, GridSpacingX, GridSpacingY, GridSpacingZ, GameObjectMatrix, ModelTransformMatrix, gameObjectID, ModelID));
	MeshList.emplace_back(mesh);

	Update(GameObjectMatrix);
	UpdateMeshPropertiesBuffer();
	for (auto& mesh : MeshList)
	{
		mesh->UpdateMeshTransformBuffer();
	}
}

void Model::UpdateModelTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList, uint32_t customIndex)
{
	for (auto& mesh : MeshList)
	{
		glm::mat4 GLMTransformMatrix2 = GameObjectTransformMatrix * ModelTransformMatrix;
		VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(GLMTransformMatrix2);

		VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
		AccelerationStructureInstance.transform = transformMatrix;
		AccelerationStructureInstance.instanceCustomIndex = customIndex; // I can see problems here later on.
		AccelerationStructureInstance.mask = 0xFF;
		AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
		AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		AccelerationStructureInstance.accelerationStructureReference = mesh->GetBLASBufferDeviceAddress();
		AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
	}
}

void Model::Update(const glm::mat4& GameObjectTransformMatrix)
{
	ModelTransformMatrix = glm::mat4(1.0f);
	ModelTransformMatrix = glm::translate(ModelTransformMatrix, ModelPosition);
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelTransformMatrix = glm::scale(ModelTransformMatrix, ModelScale);

	for (auto& mesh : MeshList)
	{
		mesh->Update(GameObjectTransformMatrix, ModelTransformMatrix);
	}
}

VkDescriptorBufferInfo Model::UpdateVertexBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
	//MeshPropertiesmBufferInfo.buffer = VertexPropertiesBuffer[);
	MeshPropertiesmBufferInfo.offset = 0;
	MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
	return MeshPropertiesmBufferInfo;
}

VkDescriptorBufferInfo Model::UpdateIndexBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
//	MeshPropertiesmBufferInfo.buffer = IndexPropertiesBuffer.GetBuffer();
	MeshPropertiesmBufferInfo.offset = 0;
	MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
	return MeshPropertiesmBufferInfo;
}

void Model::LoadLights(GLTFModelData& lights)
{
	for (auto& light : lights.SunLightList)
	{
		GLTFSceneManager::AddSunLight(light);
	}
	for (auto& light : lights.DirectionalLightList)
	{
		GLTFSceneManager::AddDirectionalLight(light);
	}
	for (auto& light : lights.PointLightList)
	{
		GLTFSceneManager::AddPointLight(light);
	}
	for (auto& light : lights.SpotLightList)
	{
		GLTFSceneManager::AddSpotLight(light);
	}
}

void Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto mesh : MeshList)
	{
		MeshPropertiesDescriptorList.emplace_back(mesh->UpdateMeshPropertiesBuffer());
	}
	MeshPropertiesBuffer = MeshPropertiesDescriptorList;
}

void Model::DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout)
{
	for (auto& mesh : MeshList)
	{
		auto lineMesh = reinterpret_cast<LineMesh3D*>(mesh.get());
		lineMesh->DrawLine(commandBuffer, descriptorSet, shaderPipelineLayout);
	}
}

void Model::Destroy()
{
	if (VertexBuffer != nullptr)
	{
		VertexBuffer->DestroyBuffer();
	}
	if (IndexBuffer != nullptr)
	{
		IndexBuffer->DestroyBuffer();
	}

	scratchBuffer.DestroyBuffer();

	for (auto& material : MeshPropertiesBufferList)
	{
		material.DestroyBuffer();
	}
	for (auto& mesh : MeshList)
	{
		mesh->Destroy();
	}
}
