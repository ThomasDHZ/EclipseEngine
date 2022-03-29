#include "Mesh.h"
#include "Math.h"

uint64_t Mesh::MeshIDCounter = 0;

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<LineVertex>& vertices)
{
	GenerateID();

	//VertexList = vertices;

	IndexList = {

	};

	ParentModelID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;
	BoneCount = 0;

	meshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Mesh::Mesh(glm::vec3& StartPoint, glm::vec3& EndPoint)
{
	GenerateID();

	std::vector<LineVertex> VertexList2 = {
	{{StartPoint}, {1.0f, 0.0f, 0.0f}},
	{{EndPoint}, {0.0f, 1.0f, 0.0f}}
	};

	IndexList = {

	};

	ParentModelID = -1;
	VertexCount = VertexList2.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;
	BoneCount = 0;

	meshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList2.data(), VertexList2.size() * sizeof(LineVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices)
{
	GenerateID();

	VertexList = vertices;
	IndexList = indices;

	ParentModelID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;

	meshType = MeshTypeEnum::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	BottomLevelAccelerationBuffer = AccelerationStructureBuffer();

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
		AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
		AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = nullptr;

		AccelerationStructureBuildRangeInfo.primitiveCount = TriangleCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		MeshBottomLevelAccelerationStructure();
	}
}

Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
{
	GenerateID();

	VertexList = vertices;
	IndexList = indices;

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;
	meshType = MeshTypeEnum::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	BottomLevelAccelerationBuffer = AccelerationStructureBuffer();

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

		MeshBottomLevelAccelerationStructure();
	}
}

Mesh::Mesh(MeshLoadingInfo& meshLoader)
{
	GenerateID();

	VertexList = meshLoader.vertices;
	IndexList = meshLoader.indices;

	ParentModelID = meshLoader.ModelID;
	VertexCount = meshLoader.vertices.size();
	IndexCount = meshLoader.indices.size();
	TriangleCount = static_cast<uint32_t>(meshLoader.indices.size()) / 3;
	BoneCount = meshLoader.BoneCount;
	meshType = meshLoader.meshType;

	material = meshLoader.materialPtr;
	glm::mat4 MeshTransform = meshLoader.MeshTransform;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	if (BoneCount != 0)
	{
		BoneWeightBuffer.CreateBuffer(BoneWeightList.data(), sizeof(MeshBoneWeights) * BoneWeightList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		BoneTransformBuffer.CreateBuffer(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
	
	BottomLevelAccelerationBuffer = AccelerationStructureBuffer();
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

		MeshBottomLevelAccelerationStructure();
	}
}


Mesh::~Mesh()
{
}

void Mesh::MeshBottomLevelAccelerationStructure()
{
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

void Mesh::UpdateMeshProperties()
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	meshProperties.MeshTransform = TransformMatrix;
	meshProperties.ModelTransform = glm::mat4(1.0f);
	meshProperties.materialBufferData = material->GetMaterialTextureData();
	MeshPropertiesBuffer.Update(meshProperties);

	MeshTransformMatrix = meshProperties.MeshTransform;
	glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);

	//if (TransformBuffer.Buffer != nullptr)
	//{
	//	VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);

	//	TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));
	//	TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	//	if (GraphicsDevice::IsRayTracingFeatureActive() &&
	//		IndexCount != 0)
	//	{
	//		MeshBottomLevelAccelerationStructure();
	//	}
	//}
}

void Mesh::UpdateMeshProperties(const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	meshProperties.MeshTransform = TransformMatrix;
	meshProperties.ModelTransform = glm::mat4(1.0f);
	meshProperties.materialBufferData = material->GetMaterialTextureData();
	MeshPropertiesBuffer.Update(meshProperties);

	if (BoneList.size() != 0)
	{
		for (auto bone : BoneList)
		{
			BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
		}
		BoneTransformBuffer.CopyBufferToMemory(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size());
	}

	MeshTransformMatrix = meshProperties.MeshTransform;
	glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);

	//if (TransformBuffer.Buffer != nullptr)
	//{
	//	VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);

	//	TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));
	//	TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	//	if (GraphicsDevice::IsRayTracingFeatureActive() &&
	//		IndexCount != 0)
	//	{
	//		MeshBottomLevelAccelerationStructure();
	//	}
	//}
}

void Mesh::GenerateID()
{
	MeshIDCounter++;
	MeshID = MeshIDCounter;
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

void Mesh::Destory()
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
	if(BoneTransformBuffer.Buffer != nullptr)
	{
		BoneTransformBuffer.DestoryBuffer();
	}
	if (BoneWeightBuffer.Buffer != nullptr)
	{
		BoneWeightBuffer.DestoryBuffer();
	}
	if (BottomLevelAccelerationBuffer.GetAccelerationStructureHandle() != VK_NULL_HANDLE)
	{
		BottomLevelAccelerationBuffer.Destroy();
	}
}

void Mesh::SetParentModel(uint64_t ModelID)
{
	ParentModelID = ModelID;
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

void Mesh::SetMeshPosition(glm::vec3 position)
{
	MeshPosition = position;
}

void Mesh::SetMeshRotation(glm::vec3 rotation)
{
	MeshRotation = rotation;
}

void Mesh::SetMeshScale(glm::vec3 scale)
{
	MeshScale = scale;
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