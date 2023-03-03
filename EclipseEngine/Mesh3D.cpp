#include "Mesh3D.h"
#include "Math.h"

Mesh3D::Mesh3D()
{
}

Mesh3D::Mesh3D(MeshLoader3D& meshLoader)
{
	ParentGameObjectID = meshLoader.ParentGameObjectID;
	ParentModelID = meshLoader.ParentModelID;
	GenerateID();
	GenerateColorID();

	GameObjectTransformMatrix = meshLoader.GameObjectTransform;
	ModelTransformMatrix = meshLoader.ModelTransform;
	MeshTransformMatrix = meshLoader.MeshTransform;

	VertexList = meshLoader.VerticeList;
	IndexList = meshLoader.IndexList;
	material = meshLoader.MaterialPtr;

	MeshType = meshLoader.MeshType;
	MeshSubType = meshLoader.MeshSubType;

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;

	LastTransform = GameObjectTransformMatrix * ModelTransformMatrix * MeshTransformMatrix;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&LastTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	RTXMeshStartUp();
	AnimationStartUp(meshLoader);
	InstancingStartUp(meshLoader.instanceData);
}

Mesh3D::~Mesh3D()
{
}

void Mesh3D::InstancingStartUp(InstancingDataStruct& instanceData)
{
	InstanceCount = instanceData.instanceMeshDataList.size();
	if (InstanceCount > 0)
	{
		for (auto& instance : instanceData.instanceMeshDataList)
		{
			InstancedData3D instanceData2;

			glm::mat4 TransformMatrix = glm::mat4(1.0f);
			TransformMatrix = glm::translate(TransformMatrix, instance.InstancePosition);
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			TransformMatrix = glm::scale(TransformMatrix, instance.InstanceScale);

			const auto matID = rand() % instanceData.MaterialList.size();

			instanceData2.InstanceModel = GameObjectTransformMatrix * ModelTransformMatrix * TransformMatrix;
			instanceData2.MaterialID = instanceData.MaterialList[matID]->GetMaterialBufferIndex();
			InstancedDataList.emplace_back(instanceData2);
		}

		InstanceBuffer.CreateBuffer(InstancedDataList.data(), InstancedDataList.size() * sizeof(InstancedData3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

void Mesh3D::AnimationStartUp(MeshLoader3D& meshLoader)
{
	BoneCount = meshLoader.BoneCount;
	if (BoneCount > 0)
	{
		BoneWeightList = meshLoader.BoneWeightList;
		BoneTransform.resize(BoneCount, glm::mat4(1.0f));

		BoneWeightBuffer.CreateBuffer(BoneWeightList.data(), sizeof(MeshBoneWeights) * BoneWeightList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		BoneTransformBuffer.CreateBuffer(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

void Mesh3D::RTXMeshStartUp()
{
	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		TransformInverseBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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

void Mesh3D::UpdateMeshBottomLevelAccelerationStructure()
{
	if (GraphicsDevice::IsRayTracingFeatureActive())
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

		VulkanBuffer scratchBuffer = VulkanBuffer(nullptr, AccelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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

void Mesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = ModelMatrix;

	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	reflectionPoint = glm::vec3(GameObjectTransformMatrix[3][0], GameObjectTransformMatrix[3][1], GameObjectTransformMatrix[3][2]) +
					  glm::vec3(ModelTransformMatrix[3][0], ModelTransformMatrix[3][1], ModelTransformMatrix[3][2]) +
					  MeshPosition + ReflectionPoint;

	meshProperties.MeshTransform = GameObjectTransformMatrix * ModelTransformMatrix * TransformMatrix;
	meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();

	if (SelectedMesh)
	{
		meshProperties.SelectedObjectBufferIndex = 1;
	}
	else
	{
		meshProperties.SelectedObjectBufferIndex = 0;
	}

	MeshPropertiesBuffer.Update(meshProperties);

//	if (LastTransform != meshProperties.MeshTransform &&
//		IndexCount != 0)
//	{
//		MeshTransformMatrix = meshProperties.MeshTransform;
//		TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));
//
//		glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
//		VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
//		TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));
//
////		UpdateMeshBottomLevelAccelerationStructure();
//	//LastTransform = meshProperties.MeshTransform;
//	}
}

void Mesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
	GameObjectTransformMatrix = GameObjectMatrix;
	ModelTransformMatrix = ModelMatrix;

	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	reflectionPoint = glm::vec3(GameObjectTransformMatrix[3][0], GameObjectTransformMatrix[3][1], GameObjectTransformMatrix[3][2]) +
		glm::vec3(ModelTransformMatrix[3][0], ModelTransformMatrix[3][1], ModelTransformMatrix[3][2]) +
		MeshPosition + ReflectionPoint;

	meshProperties.MeshTransform = GameObjectTransformMatrix * ModelTransformMatrix * TransformMatrix;
	meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();

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

	if (BoneList.size() != 0)
	{
		for (auto bone : BoneList)
		{
			BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
		}
		BoneTransformBuffer.CopyBufferToMemory(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size());
	}


	//if (VulkanRenderer::UpdateBLAS &&
	//	IndexCount != 0)
	//{

	 //   MeshTransformMatrix = meshProperties.MeshTransform;
		//TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));

		//glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
		//VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
		//TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

		//UpdateMeshBottomLevelAccelerationStructure();
	//}
}

void Mesh3D::Destroy()
{
	Mesh::Destroy();
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