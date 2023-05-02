#include "GLTF_Temp_Model.h"
#include "SceneManager.h"
#include "EngineMath.h"

uint64_t GLTF_Temp_Model::ModelIDCounter = 0;

GLTF_Temp_Model::GLTF_Temp_Model()
{
}

GLTF_Temp_Model::~GLTF_Temp_Model()
{
}

void GLTF_Temp_Model::GenerateID()
{
	ModelIDCounter++;
	ModelID = ModelIDCounter;
}

void GLTF_Temp_Model::RTXModelStartUp()
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

void GLTF_Temp_Model::LoadMaterials(std::vector<GLTFMaterialLoader>& materialLoader)
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


void GLTF_Temp_Model::UpdateModelTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList, uint32_t customIndex)
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

void GLTF_Temp_Model::Update(const glm::mat4& GameObjectTransformMatrix)
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

VkDescriptorBufferInfo GLTF_Temp_Model::UpdateVertexBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
	//MeshPropertiesmBufferInfo.buffer = VertexPropertiesBuffer[);
	MeshPropertiesmBufferInfo.offset = 0;
	MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
	return MeshPropertiesmBufferInfo;
}

VkDescriptorBufferInfo GLTF_Temp_Model::UpdateIndexBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
//	MeshPropertiesmBufferInfo.buffer = IndexPropertiesBuffer.GetBuffer();
	MeshPropertiesmBufferInfo.offset = 0;
	MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
	return MeshPropertiesmBufferInfo;
}

void GLTF_Temp_Model::LoadLights(GLTFModelData& lights)
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

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto mesh : MeshList)
	{
		MeshPropertiesDescriptorList.emplace_back(mesh->UpdateMeshPropertiesBuffer());
	}
	MeshPropertiesBuffer = MeshPropertiesDescriptorList;
}


void GLTF_Temp_Model::Draw(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	if (IndexBuffer.Buffer != nullptr)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	for (auto& mesh : MeshList)
	{
		mesh->Draw(commandBuffer, descriptorset, shaderPipelineLayout);
	}
}

void GLTF_Temp_Model::DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	if (IndexBuffer.Buffer != nullptr)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	for (auto& mesh : MeshList)
	{
		mesh->DrawMesh(commandBuffer, descriptorset, shaderPipelineLayout, sceneProperties);
	}
}

void GLTF_Temp_Model::DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	if (IndexBuffer.Buffer != nullptr)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	for (auto& mesh : MeshList)
	{
		mesh->DrawInstancedMesh(commandBuffer, descriptorset, shaderPipelineLayout, sceneProperties);
	}
}

void GLTF_Temp_Model::DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawSprite(commandBuffer, descriptorset, shaderPipelineLayout, sceneProperties);
	}
}

void GLTF_Temp_Model::DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	//vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawLine(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperties);
	}
}

void GLTF_Temp_Model::Destroy()
{
	VertexBuffer.DestroyBuffer();
	IndexBuffer.DestroyBuffer();


	for (auto& material : MeshPropertiesBufferList)
	{
		material.DestroyBuffer();
	}
	for (auto& mesh : MeshList)
	{
		mesh->Destroy();
	}
}
