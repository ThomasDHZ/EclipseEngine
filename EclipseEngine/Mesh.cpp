#include "Mesh.h"
#include "Math.h"

uint64_t Mesh::MeshIDCounter = 0;

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex>& vertices)
{
	GenerateID();

	VertexList = vertices;
	IndexList = {
0, 1, 2, 2, 3, 0
	};

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	PrimitiveCount = static_cast<uint32_t>(IndexList.size()) / 3;

	meshType = EnumMeshType::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
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

		PrimitiveCount = IndexCount / 3;

		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
		AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = nullptr;

		AccelerationStructureBuildRangeInfo.primitiveCount = PrimitiveCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		MeshBottomLevelAccelerationStructure();
	}
}

Mesh::Mesh(glm::vec3& StartPoint, glm::vec3& EndPoint)
{
	GenerateID();

	VertexList = {
		{{StartPoint}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{EndPoint}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
	};

	IndexList = {
0, 1, 2, 2, 3, 0
	};

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	PrimitiveCount = static_cast<uint32_t>(IndexList.size()) / 3;

	meshType = EnumMeshType::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
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

		PrimitiveCount = IndexCount / 3;

		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
		AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = nullptr;

		AccelerationStructureBuildRangeInfo.primitiveCount = PrimitiveCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		MeshBottomLevelAccelerationStructure();
	}
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	GenerateID();

	VertexList = vertices;
	IndexList = indices;

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	PrimitiveCount = static_cast<uint32_t>(indices.size()) / 3;

	meshType = EnumMeshType::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
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

		PrimitiveCount = IndexCount / 3;

		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
		AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = nullptr;

		AccelerationStructureBuildRangeInfo.primitiveCount = PrimitiveCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		MeshBottomLevelAccelerationStructure();
	}
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
{
	GenerateID();

	VertexList = vertices;
	IndexList = indices;

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	PrimitiveCount = static_cast<uint32_t>(indices.size()) / 3;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
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

		PrimitiveCount = IndexCount / 3;

		AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;

		AccelerationStructureGeometry.geometry.triangles.transformData = TransformInverseBufferDeviceAddress;

		AccelerationStructureBuildRangeInfo.primitiveCount = PrimitiveCount;
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
	std::vector<uint32_t> PrimitiveCountList{ PrimitiveCount };
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

	if (BottomLevelAccelerationBuffer.handle == VK_NULL_HANDLE)
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
	if (BottomLevelAccelerationBuffer.handle == VK_NULL_HANDLE)
	{
		AccelerationBuildGeometryInfo.dstAccelerationStructure = BottomLevelAccelerationBuffer.handle;
	}
	else
	{
		AccelerationBuildGeometryInfo.srcAccelerationStructure = BottomLevelAccelerationBuffer.handle;
		AccelerationBuildGeometryInfo.dstAccelerationStructure = BottomLevelAccelerationBuffer.handle;
	}

	BottomLevelAccelerationBuffer.AcclerationCommandBuffer(AccelerationBuildGeometryInfo, AccelerationBuildStructureRangeInfos);

	scratchBuffer.DestoryBuffer();
}

void Mesh::UpdateMeshProperties(MeshProperties& meshProps)
{
	meshProperties.Update(meshProps);

	glm::mat4 FinalTransform = meshProps.MeshTransform;
	glm::mat4 transformMatrix2 = glm::transpose(meshProps.MeshTransform);

	VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);

	TransformBuffer.CopyBufferToMemory(&FinalTransform, sizeof(FinalTransform));
	TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	if (GraphicsDevice::IsRayTracingFeatureActive() &&
		GraphicsDevice::IsRayTracerActive() &&
		IndexCount != 0)
	{
		MeshBottomLevelAccelerationStructure();
	}
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
	IndexBuffer.DestoryBuffer();
	TransformBuffer.DestoryBuffer();
	TransformInverseBuffer.DestoryBuffer();
	meshProperties.Destroy();

	if (BottomLevelAccelerationBuffer.handle != VK_NULL_HANDLE)
	{
		BottomLevelAccelerationBuffer.Destroy();
	}
}

void Mesh::SetBufferIndex(int bufferIndex)
{
	BufferIndex = bufferIndex;
}

void Mesh::SetMaterial(std::shared_ptr<Material> materialPtr)
{
	material = materialPtr;
}
