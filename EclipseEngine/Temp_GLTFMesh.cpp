#include "Temp_GLTFMesh.h"
#include "SceneManager.h"

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

	GameObjectTransform = meshLoader.GameObjectTransform;
	ModelTransform = meshLoader.ModelTransform;
	MeshTransform = meshLoader.node->NodeTransformMatrix;

	MeshPosition = meshLoader.node->Position;
	MeshRotation = meshLoader.node->Rotation;
	MeshScale = meshLoader.node->Scale;

	gltfMaterial = meshLoader.node->Material;

	MeshPropertiesBuffer.CreateBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	MeshTransformBuffer = meshLoader.node->TransformBuffer;
	MeshTransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
	MeshPropertiesBuffer.CopyBufferToMemory(&meshProperties, sizeof(MeshProperties));
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

VkDescriptorBufferInfo Temp_GLTFMesh::UpdateMeshPropertiesBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
	MeshPropertiesmBufferInfo.buffer = MeshPropertiesBuffer.GetBuffer();
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


void Temp_GLTFMesh::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout)
{
	for (auto& primitve : PrimitiveList)
	{
		if (primitve.IndexCount > 0)
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &MaterialList[primitve.material]->descriptorSet, 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
		}
	}
}

void Temp_GLTFMesh::DrawSprite(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &MaterialList[0]->descriptorSet, 0, nullptr);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
}

void Temp_GLTFMesh::DrawLine(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout, VkDescriptorSet descriptorSet)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
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
	MeshPropertiesBuffer.DestroyBuffer();
	MeshTransformBuffer.DestroyBuffer();
}