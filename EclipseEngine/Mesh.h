#pragma once
#include "VulkanBuffer.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
private:
	VulkanBuffer IndexBuffer;
	VulkanBuffer VertexBuffer;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);
public:

	Mesh();
	Mesh(std::vector<Vertex>& VertexList, std::vector<uint32_t>& IndexList);
	~Mesh();

	void Update();
	void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout& ShaderLayout);
	void Destory();
};

