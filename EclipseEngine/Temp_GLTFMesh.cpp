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
	MeshType = meshLoader.MeshType;
	MeshSubType = meshLoader.MeshSubType;

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

	glm::mat4 matrix = glm::mat4(1.0f);
	TransformMatrixList.emplace_back(matrix);

	VulkanBuffer TransformBuffer = meshLoader.node->TransformBuffer;
	TransformBuffer.CreateBuffer(&matrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshTransformBufferList.emplace_back(TransformBuffer);

	for (auto& childMesh : ChildMeshList)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		TransformMatrixList.emplace_back(matrix);

		VulkanBuffer TransformBuffer;
		TransformBuffer.CreateBuffer(&matrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		MeshTransformBufferList.emplace_back(TransformBuffer);
	}
}

Temp_GLTFMesh::~Temp_GLTFMesh()
{
}

void Temp_GLTFMesh::UpdateNodeTransform(std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix)
{
	glm::mat4 GlobalTransform = ParentMatrix * glm::mat4(1.0f);
	if (node == nullptr)
	{
		MeshTransformBufferList[0].CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}
	else
	{
		GlobalTransform = ParentMatrix * node->NodeTransformMatrix;
		MeshTransformBufferList[node->NodeID].CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}

	if (node == nullptr)
	{

		for (int x = 0; x < ChildMeshList.size() - 1; x++)
		{
			UpdateNodeTransform(ChildMeshList[x + 1], GlobalTransform);
		}
	}
}

void Temp_GLTFMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	UpdateNodeTransform(nullptr, GameObjectMatrix * ModelMatrix);
	MeshPropertiesBuffer.CopyBufferToMemory(&meshProperties, sizeof(MeshProperties));

	//GameObjectTransformMatrix = GameObjectMatrix;
	//ModelTransformMatrix = ModelMatrix;

	//meshProperties.MeshTransform = MeshTransformMatrix;
	//meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();

	//if (SelectedMesh)
	//{
	//	meshProperties.SelectedObjectBufferIndex = 1;
	//}
	//else
	//{
	//	meshProperties.SelectedObjectBufferIndex = 0;
	//}

	//MeshPropertiesBuffer.Update(meshProperties);

	//if (LastTransform != meshProperties.MeshTransform &&
	//	IndexCount != 0)
	//{
	//	MeshTransformMatrix = meshProperties.MeshTransform;
	//	TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));

	//	glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
	//	VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
	//	TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	//	UpdateMeshBottomLevelAccelerationStructure();
	//	LastTransform = meshProperties.MeshTransform;
	//}
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
	for (auto transformMatrix : MeshTransformBufferList)
	{
		VkDescriptorBufferInfo transformMatrixPropertiesBufferInfo = {};
		transformMatrixPropertiesBufferInfo.buffer = transformMatrix.GetBuffer();
		transformMatrixPropertiesBufferInfo.offset = 0;
		transformMatrixPropertiesBufferInfo.range = VK_WHOLE_SIZE;
		TransformDescriptorList.emplace_back(transformMatrixPropertiesBufferInfo);
	}
	TransformMatrixBuffer = TransformDescriptorList;
	return TransformMatrixBuffer;
}


void Temp_GLTFMesh::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout)
{
	for (auto& node : ChildMeshList)
	{
		if (node->Visible)
		{
			for (auto& primitve : node->PrimitiveList)
			{
				if (primitve.IndexCount > 0)
				{
					SceneManager::sceneProperites.MeshIndex = node->NodeID;
					//SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

					vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descripterSetList[node->NodeID], 0, nullptr);
					vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
					vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
				}
			}
		}
	}
}

void Temp_GLTFMesh::Destroy()
{
	MeshPropertiesBuffer.DestoryBuffer();
	for (auto& transformBuffer : MeshTransformBufferList)
	{
		transformBuffer.DestoryBuffer();
	}
}