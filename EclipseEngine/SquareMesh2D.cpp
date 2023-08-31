#include "SquareMesh2D.h"

SquareMesh2D::SquareMesh2D(const std::string& LineName, float size, const glm::vec3& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	LineVertexList =
	{
		{glm::vec2(0.0f), glm::vec3(1.0f)},
		{glm::vec2(0.0f, size), glm::vec3(1.0f)},
		{glm::vec2(size, size), glm::vec3(1.0f)},
		{glm::vec2(size, 0.0f), glm::vec3(1.0f)}
	};
	LineIndexList = {
		0, 1,
		1, 0,
		1, 2,
		2, 1,
		2, 3,
		3, 2,
		3, 4,
		4, 3,
	};

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = LineName;

	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	BoneCount = 0;
	TriangleCount = 0;
	InstanceCount = 0;

	GameObjectTransform = GameObjectMatrix;
	ModelTransform = ModelMatrix;
	MeshTransform = glm::mat4(1.0f);

	MeshPosition = glm::vec3(0.0f);
	MeshRotation = glm::vec3(0.0f);
	MeshScale = glm::vec3(1.0f);

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

SquareMesh2D::SquareMesh2D(const std::string& LineName, float size, const glm::vec4& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	LineVertexList =
	{
		{glm::vec2(0.0f, 0.0f), glm::vec3(1.0f)},
		{glm::vec2(0.0f, size), glm::vec3(1.0f)},
		{glm::vec2(size, size), glm::vec3(1.0f)},
		{glm::vec2(size, 0.0f), glm::vec3(1.0f)}
	};
	LineIndexList = {
		0, 1, 
		1, 2,
		2, 3,
		3, 0
	};


	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = LineName;

	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	BoneCount = 0;
	TriangleCount = 0;
	InstanceCount = 0;

	GameObjectTransform = GameObjectMatrix;
	ModelTransform = ModelMatrix;
	MeshTransform = glm::mat4(1.0f);

	MeshPosition = glm::vec3(0.0f);
	MeshRotation = glm::vec3(0.0f);
	MeshScale = glm::vec3(1.0f);

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

SquareMesh2D::~SquareMesh2D()
{
}

void SquareMesh2D::DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout)
{
	Mesh::DrawLine(commandBuffer, descriptorSet, shaderPipelineLayout);
}

void SquareMesh2D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(GameObjectMatrix, ModelMatrix);
}

void SquareMesh2D::Destroy()
{
	if (VertexBuffer != nullptr)
	{
		VertexBuffer->DestroyBuffer();
	}
	if (IndexBuffer != nullptr)
	{
		IndexBuffer->DestroyBuffer();
	}
	Mesh::Destroy();
}

void SquareMesh2D::SetMeshPosition(float x, float y)
{
	MeshPosition = glm::vec3(x, y, 0.0f);
}

void SquareMesh2D::SetMeshRotation(float x, float y)
{
	MeshRotation = glm::vec3(x, y, 0.0f);
}

void SquareMesh2D::SetMeshScale(float x, float y)
{
	MeshScale = glm::vec3(x, y, 0.0f);
}

void SquareMesh2D::SetMeshPosition(const glm::vec2& position)
{
	MeshPosition = glm::vec3(position.x, position.y, 0.0f);
}

void SquareMesh2D::SetMeshRotation(const glm::vec2& rotation)
{
	MeshRotation = glm::vec3(rotation.x, rotation.y, 0.0f);
}

void SquareMesh2D::SetMeshScale(const glm::vec2& scale)
{
	MeshScale = glm::vec3(scale.x, scale.y, 0.0f);
}
