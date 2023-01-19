#include "Temp_GLTFMesh.h"

Temp_GLTFMesh::Temp_GLTFMesh()
{
}

Temp_GLTFMesh::Temp_GLTFMesh(GLTFMeshLoader3D& meshLoader)
{
	this->MeshName = meshLoader.MeshName;
	MeshType = meshLoader.MeshType;
	MeshSubType = meshLoader.MeshSubType;

	ParentGameObjectID = meshLoader.ParentGameObjectID;
	ParentModelID = meshLoader.ParentModelID;
	MeshID = meshLoader.NodeID;

	ParentMeshID = meshLoader.ParentMeshID;
	ChildMeshIDList = meshLoader.ChildMeshIDList;

	Primitive = meshLoader.Primitive;

	VertexCount = meshLoader.VertexCount;
	IndexCount = meshLoader.IndexCount;
	BoneCount = meshLoader.BoneCount;

	GameObjectTransform = meshLoader.GameObjectTransform;
	ModelTransform = meshLoader.ModelTransform;
	MeshTransform = meshLoader.MeshTransform;

	MeshPosition = meshLoader.MeshPosition;
	MeshRotation = meshLoader.MeshRotation;
	MeshScale = meshLoader.MeshScale;

	TransformBuffer = meshLoader.TransformBuffer;
	
	material = meshLoader.MaterialPtr;
}

Temp_GLTFMesh::~Temp_GLTFMesh()
{
}

void Temp_GLTFMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
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

	MeshTransformMatrix = meshProperties.MeshTransform;
	TransformBuffer.CopyBufferToMemory(&MeshTransformMatrix, sizeof(MeshTransformMatrix));

	//glm::mat4 transformMatrix2 = glm::transpose(meshProperties.MeshTransform);
	//VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(transformMatrix2);
	//TransformInverseBuffer.CopyBufferToMemory(&transformMatrix, sizeof(transformMatrix));

	//UpdateMeshBottomLevelAccelerationStructure();
	//}
}

void Temp_GLTFMesh::Draw(VkCommandBuffer& commandBuffer)
{
	if (IndexCount > 0)
	{
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, Primitive.FirstIndex, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
}

void Temp_GLTFMesh::Destroy()
{
	Mesh::Destroy();
	//if (TransformBuffer.Buffer != nullptr)
	//{
	//	TransformBuffer.DestoryBuffer();
	//}
	//if (TransformInverseBuffer.Buffer != nullptr)
	//{
	//	TransformInverseBuffer.DestoryBuffer();
	//}
	//if (BoneTransformBuffer.Buffer != nullptr)
	//{
	//	BoneTransformBuffer.DestoryBuffer();
	//}
	//if (BoneWeightBuffer.Buffer != nullptr)
	//{
	//	BoneWeightBuffer.DestoryBuffer();
	//}
}
