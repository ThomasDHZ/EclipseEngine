#pragma once
#include "VulkanRenderer.h"
#include "UniformBuffer.h"

class InstanceMesh
{
private:


public:
	InstanceMesh();
	InstanceMesh(uint64_t InstMeshID);
	~InstanceMesh();

	uint64_t MeshID;
	uint32_t MeshBufferIndex;

	glm::vec3 InstancePosition = glm::vec3(0.0f);
	glm::vec3 InstanceRotation = glm::vec3(0.0f);
	glm::vec3 InstanceScale = glm::vec3(1.0f);

	glm::mat4 InstanceMeshTransform;
	//std::shared_ptr<Material> material;

	MeshProperties meshProperties;
	MeshPropertiesUniformBuffer MeshPropertiesBuffer;

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Destroy();

	void GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList);
	void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList, VkBuffer VertexBuffer);
	void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList, VkBuffer IndexBuffer);
};
