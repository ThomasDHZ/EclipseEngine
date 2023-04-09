#include "GLTF_Temp_Model.h"
#include "SceneManager.h"

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

void GLTF_Temp_Model::UpdateTopLevelAccelerationStructure()
{

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};


		for (int x = 0; x < GameObjectManager::GetModelRendererGameObjects().size(); x++)
		{
			const auto modelRenderer = static_cast<ModelRenderer*>(GameObjectManager::GetModelRendererGameObjects()[x].get());
			modelRenderer->GetModel()->UpdateMeshTopLevelAccelerationStructure(modelRenderer->GetGameObjectMatrix(), AccelerationStructureInstanceList);
		}

		VulkanBuffer InstanceBuffer = VulkanBuffer(AccelerationStructureInstanceList.data(), sizeof(VkAccelerationStructureInstanceKHR) * AccelerationStructureInstanceList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstanceBuffer.GetBuffer());

		VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
		AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
		AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

		VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo2 = {};
		AccelerationStructureBuildGeometryInfo2.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		AccelerationStructureBuildGeometryInfo2.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		AccelerationStructureBuildGeometryInfo2.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
		AccelerationStructureBuildGeometryInfo2.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
		AccelerationStructureBuildGeometryInfo2.geometryCount = 1;
		AccelerationStructureBuildGeometryInfo2.pGeometries = &AccelerationStructureGeometry;
		AccelerationStructureBuildGeometryInfo2.scratchData.deviceAddress = scratchBuffer.GetBufferDeviceAddress();

		if (TopLevelAccelerationStructure.GetAccelerationStructureHandle() == VK_NULL_HANDLE)
		{
			AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
		}
		else
		{
			AccelerationStructureBuildGeometryInfo2.srcAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
			AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
		}

		VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo = {};
		AccelerationStructureBuildRangeInfo.primitiveCount = static_cast<uint32_t>(AccelerationStructureInstanceList.size());
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;
		std::vector<VkAccelerationStructureBuildRangeInfoKHR> AccelerationStructureBuildRangeInfoList = { AccelerationStructureBuildRangeInfo };

		TopLevelAccelerationStructure.AccelerationCommandBuffer(AccelerationStructureBuildGeometryInfo2, AccelerationStructureBuildRangeInfoList);
		InstanceBuffer.DestroyBuffer();
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

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto mesh : MeshList)
	{
		MeshPropertiesDescriptorList.emplace_back(mesh->UpdateMeshPropertiesBuffer());
	}
	MeshPropertiesBuffer = MeshPropertiesDescriptorList;
}

void GLTF_Temp_Model::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	if (IndexBuffer.Buffer != nullptr)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	for (auto& mesh : MeshList)
	{
		mesh->Draw(commandBuffer, shaderPipelineLayout);
	}
}

void GLTF_Temp_Model::DrawSprite(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawSprite(commandBuffer, shaderPipelineLayout);
	}
}

void GLTF_Temp_Model::DrawLine(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout, VkDescriptorSet descriptorSet)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	//vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawLine(commandBuffer, shaderPipelineLayout, descriptorSet);
	}
}

void GLTF_Temp_Model::Destroy()
{
	VertexBuffer.DestroyBuffer();
	IndexBuffer.DestroyBuffer();

	for (auto& texture : TextureList)
	{
		texture->Destroy();
	}
	for (auto& material : MeshPropertiesBufferList)
	{
		material.DestroyBuffer();
	}
	for (auto& mesh : MeshList)
	{
		mesh->Destroy();
	}
}
