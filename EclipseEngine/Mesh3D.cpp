#include "Mesh3D.h"
#include "Math.h"

Mesh3D::Mesh3D() : Mesh(MeshTypeEnum::kPolygon, 0)
{
}

Mesh3D::Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices)
{
	GenerateID();
	GenerateColorID();

	VertexList = vertices;
	IndexList = indices;

	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::transpose(MeshTransformMatrix);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(MeshVertex);
		AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;
		AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
		AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = nullptr;

		AccelerationStructureBuildRangeInfo.primitiveCount = TriangleCount;
		AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
		AccelerationStructureBuildRangeInfo.firstVertex = 0;
		AccelerationStructureBuildRangeInfo.transformOffset = 0;

		UpdateMeshBottomLevelAccelerationStructure();
	}
}

Mesh3D::Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
{
	GenerateID();
	GenerateColorID();

	VertexList = vertices;
	IndexList = indices;

	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(indices.size()) / 3;
	BoneCount = 0;
	MeshType = MeshTypeEnum::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::transpose(MeshTransformMatrix);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(MeshVertex);
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

Mesh3D::Mesh3D(MeshLoadingInfo& meshLoader)
{
	GenerateID();
	GenerateColorID();

	VertexList = meshLoader.vertices;
	IndexList = meshLoader.indices;

	ParentModelID = meshLoader.ModelID;
	ParentGameObjectID = meshLoader.GameObjectID;
	VertexCount = meshLoader.vertices.size();
	IndexCount = meshLoader.indices.size();
	TriangleCount = static_cast<uint32_t>(meshLoader.indices.size()) / 3;
	BoneCount = meshLoader.BoneCount;
	MeshType = meshLoader.meshType;

	material = meshLoader.materialPtr;

	MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::transpose(MeshTransformMatrix);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//if (BoneCount != 0)
	//{
	//	BoneWeightBuffer.CreateBuffer(BoneWeightList.data(), sizeof(MeshBoneWeights) * BoneWeightList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	//	BoneTransformBuffer.CreateBuffer(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	//}

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
		AccelerationStructureGeometry.geometry.triangles.vertexStride = sizeof(MeshVertex);
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

Mesh3D::~Mesh3D()
{
}

void Mesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(ModelMatrix, MeshPosition);
	TransformMatrix = glm::rotate(ModelMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(ModelMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(ModelMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(ModelMatrix, MeshScale);

	if (meshProperties.MeshTransform != TransformMatrix)
	{
		VulkanRenderer::UpdateBLAS = true;
	}

	glm::vec2 XNearFar = glm::vec2(-3.0f, 3.0f);
	glm::vec2 YNearFar = glm::vec2(-3.0f, 3.0f);
	glm::vec2 ZNearFar = glm::vec2(-3.0f, 3.0f);

	glm::mat4 reflectionProj = glm::ortho(XNearFar.x, XNearFar.y, YNearFar.x, YNearFar.y, ZNearFar.x, ZNearFar.y);
	meshProperties.MeshReflectionMatrix[0] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[1] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[2] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	meshProperties.MeshReflectionMatrix[3] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	meshProperties.MeshReflectionMatrix[4] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[5] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	meshProperties.MeshTransform = TransformMatrix;
	meshProperties.ModelTransform = ModelMatrix;
	meshProperties.GameObjectTransform = GameObjectMatrix;
	meshProperties.materialBufferData = material->GetMaterialTextureData();

	if (SelectedMesh)
	{
		meshProperties.SelectedObjectBufferIndex = 1;
	}
	else
	{
		meshProperties.SelectedObjectBufferIndex = 0;
	}

	MeshPropertiesBuffer.Update(meshProperties);

	MeshTransformMatrix = meshProperties.MeshTransform;

	TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));

	glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
	VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
	TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	if (VulkanRenderer::UpdateBLAS &&
		IndexCount != 0)
	{
		UpdateMeshBottomLevelAccelerationStructure();
	}
}

void Mesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	glm::vec2 XNearFar = glm::vec2(-10.0f, 10.0f);
	glm::vec2 YNearFar = glm::vec2(-10.0f, 10.0f);
	glm::vec2 ZNearFar = glm::vec2(-10.0f, 10.0f);

	glm::mat4 reflectionProj = glm::ortho(XNearFar.x, XNearFar.y, YNearFar.x, YNearFar.y, ZNearFar.x, ZNearFar.y);
	meshProperties.MeshReflectionMatrix[0] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[1] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[2] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	meshProperties.MeshReflectionMatrix[3] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	meshProperties.MeshReflectionMatrix[4] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	meshProperties.MeshReflectionMatrix[5] = reflectionProj * glm::lookAt(MeshPosition, MeshPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	meshProperties.MeshTransform = TransformMatrix;
	meshProperties.ModelTransform = ModelMatrix;
	meshProperties.GameObjectTransform = GameObjectMatrix;
	meshProperties.materialBufferData = material->GetMaterialTextureData();

	if (SelectedMesh)
	{
		meshProperties.SelectedObjectBufferIndex = 1;
	}
	else
	{
		meshProperties.SelectedObjectBufferIndex = 0;
	}
	MeshPropertiesBuffer.Update(meshProperties);

	//if (BoneList.size() != 0)
	//{
	//	for (auto bone : BoneList)
	//	{
	//		BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
	//	}
	//	BoneTransformBuffer.CopyBufferToMemory(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size());
	//}

	MeshTransformMatrix = meshProperties.MeshTransform;
	glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);

	if (GraphicsDevice::IsRayTracerActive() &&
		IndexCount != 0)
	{
		VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
		TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));
		TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));
		UpdateMeshBottomLevelAccelerationStructure();
	}
}

void Mesh3D::SetMeshPosition(float x, float y, float z)
{
	MeshPosition = glm::vec3(x,y,z);
}


void Mesh3D::SetMeshRotation(float x, float y, float z)
{
	MeshRotation = glm::vec3(x, y, z);
}

void Mesh3D::SetMeshScale(float x, float y, float z)
{
	MeshScale = glm::vec3(x, y, z);
}

void Mesh3D::SetMeshPosition(glm::vec3 position)
{
	MeshPosition = position;
}

void Mesh3D::SetMeshRotation(glm::vec3 rotation)
{
	MeshRotation = rotation;
}

void Mesh3D::SetMeshScale(glm::vec3 scale)
{
	MeshScale = scale;
}