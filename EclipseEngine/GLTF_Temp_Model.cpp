#include "GLTF_Temp_Model.h"
#include "SceneManager.h"

uint64_t GLTF_Temp_Model::ModelIDCounter = 0;

GLTF_Temp_Model::GLTF_Temp_Model()
{
}

GLTF_Temp_Model::~GLTF_Temp_Model()
{
}

void GLTF_Temp_Model::GenerateID()
{
	ModelIDCounter++;
	ModelID = ModelIDCounter;
}

void GLTF_Temp_Model::Update(const glm::mat4& GameObjectTransformMatrix)
{
	ModelTransformMatrix = glm::mat4(1.0f);
	ModelTransformMatrix = glm::translate(ModelTransformMatrix, ModelPosition);
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelTransformMatrix = glm::scale(ModelTransformMatrix, ModelScale);

	for (auto& mesh : MeshList)
	{
		mesh->Update(GameObjectTransformMatrix, ModelTransformMatrix);
	}
}

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto mesh : MeshList)
	{
		MeshPropertiesDescriptorList.emplace_back(mesh->UpdateMeshPropertiesBuffer());
	}
	MeshPropertiesBuffer = MeshPropertiesDescriptorList;
}

void GLTF_Temp_Model::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	if (IndexBuffer.Buffer != nullptr)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	for (auto& mesh : MeshList)
	{
		mesh->Draw(commandBuffer, shaderPipelineLayout);
	}
}

void GLTF_Temp_Model::DrawSprite(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawSprite(commandBuffer, shaderPipelineLayout);
	}
}

void GLTF_Temp_Model::DrawLine(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout, VkDescriptorSet descriptorSet)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	//vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		mesh->DrawLine(commandBuffer, shaderPipelineLayout, descriptorSet);
	}
}

void GLTF_Temp_Model::Destroy()
{
	VertexBuffer.DestroyBuffer();
	IndexBuffer.DestroyBuffer();

	for (auto& texture : TextureList)
	{
		texture->Destroy();
	}
	for (auto& material : MeshPropertiesBufferList)
	{
		material.DestroyBuffer();
	}
	for (auto& mesh : MeshList)
	{
		mesh->Destroy();
	}
}
