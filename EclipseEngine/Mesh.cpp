#include "Mesh.h"
#include "Math.h"

uint64_t Mesh::MeshIDCounter = 0;

Mesh::Mesh()
{
}

Mesh::Mesh(MeshTypeEnum meshType, uint64_t parentGameObjectID)
{
	MeshType = meshType;
	ParentGameObjectID = parentGameObjectID;
}

Mesh::~Mesh()
{
}

void Mesh::UpdateMeshBottomLevelAccelerationStructure()
{
	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VkDeviceOrHostAddressConstKHR VertexBufferDeviceAddress;
		VkDeviceOrHostAddressConstKHR IndexBufferDeviceAddress;
		VkDeviceOrHostAddressConstKHR TransformInverseBufferDeviceAddress;

		VertexBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(VertexBuffer.GetBuffer());
		IndexBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(IndexBuffer.GetBuffer());
		TransformInverseBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(TransformInverseBuffer.GetBuffer());

		TriangleCount = IndexCount / 3;

		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(MeshVertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;

		AccelerationStructureGeometry.geometry.triangles.transformData = TransformInverseBufferDeviceAddress;

		AccelerationStructureBuildRangeInfo.primitiveCount = TriangleCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		std::vector<uint32_t> PrimitiveCountList{ TriangleCount };
		std::vector<VkAccelerationStructureGeometryKHR> AccelerationStructureGeometryList{ AccelerationStructureGeometry };
		std::vector<VkAccelerationStructureBuildRangeInfoKHR> AccelerationBuildStructureRangeInfos{ AccelerationStructureBuildRangeInfo };

		VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo = {};
		AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;;
		AccelerationStructureBuildGeometryInfo.geometryCount = static_cast<uint32_t>(AccelerationStructureGeometryList.size());
		AccelerationStructureBuildGeometryInfo.pGeometries = AccelerationStructureGeometryList.data();

		PrimitiveCountList.resize(AccelerationBuildStructureRangeInfos.size());
		for (auto x = 0; x < AccelerationBuildStructureRangeInfos.size(); x++)
		{
			PrimitiveCountList[x] = AccelerationBuildStructureRangeInfos[x].primitiveCount;
		}

		VkAccelerationStructureBuildSizesInfoKHR AccelerationStructureBuildSizesInfo = {};
		AccelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
		VulkanRenderer::vkGetAccelerationStructureBuildSizesKHR(VulkanRenderer::GetDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &AccelerationStructureBuildGeometryInfo, PrimitiveCountList.data(), &AccelerationStructureBuildSizesInfo);

		if (BottomLevelAccelerationBuffer.GetAccelerationStructureHandle() == VK_NULL_HANDLE)
		{
			BottomLevelAccelerationBuffer.CreateAccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR, AccelerationStructureBuildSizesInfo);
		}

		VulkanBuffer scratchBuffer = VulkanBuffer(nullptr, AccelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		scratchBuffer.SetBufferAddress(VulkanRenderer::GetBufferDeviceAddress(scratchBuffer.GetBuffer()));

		VkAccelerationStructureBuildGeometryInfoKHR AccelerationBuildGeometryInfo = {};
		AccelerationBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		AccelerationBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		AccelerationBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
		AccelerationBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
		AccelerationBuildGeometryInfo.geometryCount = static_cast<uint32_t>(AccelerationStructureGeometryList.size());
		AccelerationBuildGeometryInfo.pGeometries = AccelerationStructureGeometryList.data();
		AccelerationBuildGeometryInfo.scratchData.deviceAddress = scratchBuffer.GetBufferDeviceAddress();
		if (BottomLevelAccelerationBuffer.GetAccelerationStructureHandle() == VK_NULL_HANDLE)
		{
			AccelerationBuildGeometryInfo.dstAccelerationStructure = BottomLevelAccelerationBuffer.GetAccelerationStructureHandle();
		}
		else
		{
			AccelerationBuildGeometryInfo.srcAccelerationStructure = BottomLevelAccelerationBuffer.GetAccelerationStructureHandle();
			AccelerationBuildGeometryInfo.dstAccelerationStructure = BottomLevelAccelerationBuffer.GetAccelerationStructureHandle();
		}

		BottomLevelAccelerationBuffer.AccelerationCommandBuffer(AccelerationBuildGeometryInfo, AccelerationBuildStructureRangeInfos);

		scratchBuffer.DestoryBuffer();
	}
}

void Mesh::GenerateID()
{
	MeshIDCounter++;
	MeshID = MeshIDCounter;
}

void Mesh::GenerateColorID()
{
	byte red = (255.0f - (float)MeshID);
	byte green = (255.0f - (float)MeshID);
	byte blue = (255.0f - (float)MeshID);

	MeshColorID = Pixel(red, green, blue, 0xFF);
}

void Mesh::Draw(VkCommandBuffer& commandBuffer)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, VertexBuffer.GetBufferPtr(), offsets);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
}

void Mesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
}

void Mesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
}

void Mesh::Destroy()
{
	VertexBuffer.DestoryBuffer();
	MeshPropertiesBuffer.Destroy();

	if (IndexBuffer.Buffer != nullptr)
	{
		IndexBuffer.DestoryBuffer();
	}
	if (TransformBuffer.Buffer != nullptr)
	{
		TransformBuffer.DestoryBuffer();
	}
	if (TransformInverseBuffer.Buffer != nullptr)
	{
		TransformInverseBuffer.DestoryBuffer();
	}
	//if(BoneTransformBuffer.Buffer != nullptr)
	//{
	//	BoneTransformBuffer.DestoryBuffer();
	//}
	//if (BoneWeightBuffer.Buffer != nullptr)
	//{
	//	BoneWeightBuffer.DestoryBuffer();
	//}
	if (BottomLevelAccelerationBuffer.GetAccelerationStructureHandle() != VK_NULL_HANDLE)
	{
		BottomLevelAccelerationBuffer.Destroy();
	}
}

void Mesh::SetParentModel(uint64_t ModelID)
{
	ParentModelID = ModelID;
}
void Mesh::SetSelectedMesh(bool selected)
{
	SelectedMesh = selected;
}
void Mesh::SetParentGameObjectID(uint64_t GameObjectID)
{
	ParentGameObjectID = GameObjectID;
}

void Mesh::SetBufferIndex(int bufferIndex)
{

	if (VulkanRenderer::UpdateRendererFlag)
	{
		BufferIndex = bufferIndex;
	}
	else
	{
		std::cout << "Can't update BufferIndex unless pipelines in the process of being rebuild." << std::endl;
	}
}

void Mesh::SetMaterial(std::shared_ptr<Material> materialPtr)
{
	material = materialPtr;
}

void Mesh::SetMaterial(const std::string& MaterialPath)
{
	material = MaterialManager::LoadMaterial(MaterialPath);
}

void  Mesh::GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = MeshPropertiesBuffer.GetVulkanBufferData().Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = MeshPropertiesBufferList.size() - 1;
}

void Mesh::GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = VertexBuffer.Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = VertexBufferList.size() - 1;
}

void Mesh::GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = IndexBuffer.Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = IndexBufferList.size() - 1;
}