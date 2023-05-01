#include "Temp_GLTFMesh.h"
#include "SceneManager.h"
#include "EngineMath.h"
#include "RandomNumber.h"

uint64_t Temp_GLTFMesh::MeshIDCounter = 0;

Temp_GLTFMesh::Temp_GLTFMesh()
{
}

Temp_GLTFMesh::Temp_GLTFMesh(GLTFMeshLoader3D& meshLoader)
{
	MeshID = meshLoader.node->NodeID;
	ParentModelID = meshLoader.ParentModelID;
	ParentGameObjectID = meshLoader.ParentGameObjectID;

	MeshName = meshLoader.node->NodeName;

	ParentMesh = meshLoader.node->ParentMesh;
	ChildMeshList = meshLoader.node->ChildMeshList;

	PrimitiveList = meshLoader.node->PrimitiveList;

	VertexCount = meshLoader.VertexCount;
	IndexCount = meshLoader.IndexCount;
	//BoneCount = meshLoader.node->BoneCount;
	TriangleCount = IndexCount / 3;
	InstanceCount = meshLoader.InstanceData.InstanceMeshDataList.size();

	GameObjectTransform = meshLoader.GameObjectTransform;
	ModelTransform = meshLoader.ModelTransform;
	MeshTransform = meshLoader.node->NodeTransformMatrix;

	MeshPosition = meshLoader.node->Position;
	MeshRotation = meshLoader.node->Rotation;
	MeshScale = meshLoader.node->Scale;

	gltfMaterialList = meshLoader.gltfMaterialList;

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//MeshTransformBuffer = meshLoader.node->TransformBuffer;
	MeshTransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	glm::mat4 inverseTransformMatrix = glm::transpose(meshProperties.MeshTransform);
	VkTransformMatrixKHR inverseMatrix = EngineMath::GLMToVkTransformMatrix(inverseTransformMatrix);
	MeshTransformInverseBuffer.CreateBuffer(&inverseMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	InstancingStartUp(meshLoader.InstanceData);
	RTXMeshStartUp(meshLoader.VertexBuffer, meshLoader.IndexBuffer);
}

Temp_GLTFMesh::~Temp_GLTFMesh()
{
}

void Temp_GLTFMesh::UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix)
{
	glm::mat4 GlobalTransform = ParentMatrix * glm::mat4(1.0f);
	if (node == nullptr)
	{
		MeshTransformBuffer.CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}
	else
	{
		GlobalTransform = ParentMatrix * node->NodeTransformMatrix;
		MeshTransformBuffer.CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}

	if (node == nullptr)
	{
		if (ChildMeshList.size() > 0)
		{
			for (int x = 0; x < ChildMeshList.size() - 1; x++)
			{
				UpdateNodeTransform(ChildMeshList[x + 1], GlobalTransform);
			}
		}
	}
}

void Temp_GLTFMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectMatrix * ModelMatrix * MeshMatrix);
	//MeshPropertiesBuffer.CopyBufferToMemory(&meshProperties, sizeof(MeshProperties));


	if(IndexCount != 0)
	{
		glm::mat4 inverseTransformMatrix = glm::transpose(meshProperties.MeshTransform);
		VkTransformMatrixKHR inverseMatrix = EngineMath::GLMToVkTransformMatrix(inverseTransformMatrix);
		MeshTransformInverseBuffer.CopyBufferToMemory(&inverseMatrix, sizeof(inverseMatrix));

		UpdateMeshBottomLevelAccelerationStructure();
	}
}

void Temp_GLTFMesh::UpdateMeshBufferIndex(uint64_t bufferIndex)
{
	MeshBufferIndex = bufferIndex;
}

void Temp_GLTFMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
	//GameObjectTransformMatrix = GameObjectMatrix;
	//ModelTransformMatrix = ModelMatrix;

	//glm::mat4 TransformMatrix = glm::mat4(1.0f);
	//TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	//TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//TransformMatrix = glm::scale(TransformMatrix, MeshScale);

	//reflectionPoint = glm::vec3(GameObjectTransformMatrix[3][0], GameObjectTransformMatrix[3][1], GameObjectTransformMatrix[3][2]) +
	//	glm::vec3(ModelTransformMatrix[3][0], ModelTransformMatrix[3][1], ModelTransformMatrix[3][2]) +
	//	MeshPosition + ReflectionPoint;

	//meshProperties.MeshTransform = GameObjectTransformMatrix * ModelTransformMatrix * TransformMatrix;
	////meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();

	//if (SelectedMesh)
	//{
	//	meshProperties.SelectedObjectBufferIndex = 1;
	//}
	//else
	//{
	//	meshProperties.SelectedObjectBufferIndex = 0;
	//}

	//MeshPropertiesBuffer.Update(meshProperties);

	//MeshTransformMatrix = meshProperties.MeshTransform;

	//if (BoneList.size() != 0)
	//{
	//	for (auto bone : BoneList)
	//	{
	//		BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
	//	}
	//	BoneTransformBuffer.CopyBufferToMemory(BoneTransform.data(), sizeof(glm::mat4) * BoneTransform.size());
	//}


	//if (VulkanRenderer::UpdateBLAS &&
	//	IndexCount != 0)
	//{

	//MeshTransformMatrix = meshProperties.MeshTransform;
	//TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));

	//glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
	//VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
	//TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	//UpdateMeshBottomLevelAccelerationStructure();
	//}
}

void Temp_GLTFMesh::RTXMeshStartUp(VulkanBuffer& VertexBuffer, VulkanBuffer& IndexBuffer)
{
	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		MeshTransformInverseBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BottomLevelAccelerationBuffer = AccelerationStructureBuffer();

		VkDeviceOrHostAddressConstKHR VertexBufferDeviceAddress{};
		VkDeviceOrHostAddressConstKHR IndexBufferDeviceAddress{};
		VkDeviceOrHostAddressConstKHR TransformInverseBufferDeviceAddress{};

		VertexBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(VertexBuffer.GetBuffer());
		IndexBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(IndexBuffer.GetBuffer());
		TransformInverseBufferDeviceAddress.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(MeshTransformInverseBuffer.GetBuffer());

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

void Temp_GLTFMesh::InstancingStartUp(GLTFInstancingDataStruct& instanceData)
{
	InstanceCount = instanceData.InstanceMeshDataList.size();
	if (InstanceCount > 0)
	{
		for (auto& instance : instanceData.InstanceMeshDataList)
		{
			InstancedVertexData3D instancVertexeData;

			glm::mat4 TransformMatrix = glm::mat4(1.0f);
			TransformMatrix = glm::translate(TransformMatrix, instance.InstancePosition);
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instance.InstanceRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			TransformMatrix = glm::scale(TransformMatrix, instance.InstanceScale);

			const auto matID = rand() % instanceData.MaterialList.size();

			instancVertexeData.InstanceModel = GameObjectTransform * ModelTransform * TransformMatrix;
			instancVertexeData.MaterialID = instanceData.MaterialList[matID]->GetMaterialBufferIndex();
			InstancedVertexDataList.emplace_back(instancVertexeData);
		}

		InstanceBuffer.CreateBuffer(InstancedVertexDataList.data(), InstancedVertexDataList.size() * sizeof(InstancedVertexData3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

VkDescriptorBufferInfo Temp_GLTFMesh::UpdateMeshPropertiesBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
	MeshPropertiesmBufferInfo.buffer = MeshPropertiesBuffer.Buffer;
	MeshPropertiesmBufferInfo.offset = 0;
	MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
	return MeshPropertiesmBufferInfo;
}

std::vector<VkDescriptorBufferInfo> Temp_GLTFMesh::UpdateMeshTransformBuffer()
{
	std::vector<VkDescriptorBufferInfo> TransformDescriptorList;
	VkDescriptorBufferInfo transformMatrixPropertiesBufferInfo = {};
	transformMatrixPropertiesBufferInfo.buffer = MeshTransformBuffer.GetBuffer();
	transformMatrixPropertiesBufferInfo.offset = 0;
	transformMatrixPropertiesBufferInfo.range = VK_WHOLE_SIZE;
	TransformDescriptorList.emplace_back(transformMatrixPropertiesBufferInfo);
	TransformMatrixBuffer = TransformDescriptorList;
	return TransformMatrixBuffer;
}

void Temp_GLTFMesh::UpdateMeshBottomLevelAccelerationStructure()
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

		scratchBuffer.DestroyBuffer();
	}
}

void Temp_GLTFMesh::Draw(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout ShaderPipelineLayout)
{
	for (auto& primitve : PrimitiveList)
	{
		if (primitve.IndexCount > 0)
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
		}
	}
}

void Temp_GLTFMesh::DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	for (auto& primitve : PrimitiveList)
	{
		sceneProperties.MeshIndex = MeshBufferIndex;
		sceneProperties.MaterialIndex = gltfMaterialList[primitve.material]->GetMaterialBufferIndex();
		vkCmdPushConstants(commandBuffer, shaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
		if (primitve.IndexCount > 0)
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
		}
	}
}

void Temp_GLTFMesh::DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 1, 1, InstanceBuffer.GetBufferPtr(), offsets);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, IndexCount, InstanceCount, 0, 0, 0);
	}
}

void Temp_GLTFMesh::DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	vkCmdPushConstants(commandBuffer, shaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
	//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &MaterialList[0]->descriptorSet, 0, nullptr);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
}

void Temp_GLTFMesh::DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	vkCmdPushConstants(commandBuffer, shaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
}

void Temp_GLTFMesh::Destroy()
{
	//MeshPropertiesBuffer.DestroyBuffer();
	MeshTransformBuffer.DestroyBuffer();
	BottomLevelAccelerationBuffer.Destroy();
}