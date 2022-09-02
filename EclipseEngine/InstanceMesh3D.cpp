#include "InstanceMesh3D.h"
#include "Math.h"

InstanceMesh3D::InstanceMesh3D() : Mesh(MeshTypeEnum::kPolygon, MeshSubTypeEnum::kNormal, 0)
{
}

InstanceMesh3D::InstanceMesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::vector<InstancingDataStruct> instanceData, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kPolygon, meshSubType, parentGameObjectID)
{
	MeshStartUp(vertices, indices, instanceData, parentGameObjectID);
}

InstanceMesh3D::InstanceMesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::vector<InstancingDataStruct> instanceData, std::shared_ptr<Material> materialPtr, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kPolygon, meshSubType, parentGameObjectID)
{
	MeshStartUp(vertices, indices, instanceData, materialPtr, parentGameObjectID);
}

InstanceMesh3D::InstanceMesh3D(MeshLoadingInfo& meshLoader)
{
	MeshStartUp(meshLoader);
}

InstanceMesh3D::~InstanceMesh3D()
{
}

void InstanceMesh3D::MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::vector<InstancingDataStruct> instanceData, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	VertexList = vertices;
	IndexList = indices;

	MeshType = MeshTypeEnum::kPolygon;
	material = MaterialManager::GetDefaultMaterial();
	ParentModelID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	InstanceBuffer.CreateBuffer(InstancedDataList.data(), InstancedDataList.size() * sizeof(InstancedData3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	InstancingStartUp();
	RTXMeshStartUp();
	MeshRendererManager::AddMesh(std::make_shared<Mesh>(*this));
}

void InstanceMesh3D::MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::vector<InstancingDataStruct> instanceData, std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	VertexList = vertices;
	IndexList = indices;

	ParentModelID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;
	MeshType = MeshTypeEnum::kPolygon;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	InstanceBuffer.CreateBuffer(InstancedDataList.data(), InstancedDataList.size() * sizeof(InstancedData3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	InstancingStartUp(instanceData);
	RTXMeshStartUp();
	MeshRendererManager::AddMesh(std::make_shared<Mesh>(*this));
}

void InstanceMesh3D::MeshStartUp(MeshLoadingInfo& meshLoader)
{
	GenerateID();
	GenerateColorID();

	VertexList = meshLoader.vertices;
	IndexList = meshLoader.indices;

	ParentModelID = meshLoader.ModelID;
	VertexCount = meshLoader.vertices.size();
	IndexCount = meshLoader.indices.size();
	TriangleCount = static_cast<uint32_t>(meshLoader.indices.size()) / 3;
	BoneCount = meshLoader.BoneCount;
	MeshType = meshLoader.meshType;
	material = meshLoader.materialPtr;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	InstanceBuffer.CreateBuffer(InstancedDataList.data(), InstancedDataList.size() * sizeof(InstancedData3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//InstancingStartUp(meshLoader.instanceData);
	RTXMeshStartUp();
	MeshRendererManager::AddMesh(std::make_shared<Mesh>(*this));
}

void InstanceMesh3D::InstancingStartUp(std::vector<InstancingDataStruct>& instanceDataVectors)
{
	
	for (auto& instance : InstanceDataVectors)
	{

	}
}

void InstanceMesh3D::RTXMeshStartUp()
{
	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		BottomLevelAccelerationBuffer = AccelerationStructureBuffer();

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
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex3D);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;

		AccelerationStructureGeometry.geometry.triangles.transformData = TransformInverseBufferDeviceAddress;

		AccelerationStructureBuildRangeInfo.primitiveCount = TriangleCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		UpdateMeshBottomLevelAccelerationStructure();
	}
}

void InstanceMesh3D::UpdateMeshBottomLevelAccelerationStructure()
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
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(Vertex3D);
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

void InstanceMesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
}

void InstanceMesh3D::Destroy()
{
	Mesh::Destroy();
	if (TransformBuffer.Buffer != nullptr)
	{
		TransformBuffer.DestoryBuffer();
	}
	if (InstanceBuffer.Buffer != nullptr)
	{
		InstanceBuffer.DestoryBuffer();
	}
	if (TransformInverseBuffer.Buffer != nullptr)
	{
		TransformInverseBuffer.DestoryBuffer();
	}
}