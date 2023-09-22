#include "GridMesh2D.h"
#include <glm/ext/matrix_transform.hpp>

GridMesh2D::GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { glm::vec4(1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { glm::vec4(1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { glm::vec4(1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { glm::vec4(1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = GridName;

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

	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectTransform * ModelTransform * MeshMatrix);
}

GridMesh2D::GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec3& LineColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = GridName;

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

	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectTransform * ModelTransform * MeshMatrix);
}

GridMesh2D::GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec4& LineColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { LineColor }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { LineColor }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { LineColor }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { LineColor }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = GridName;

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

	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectTransform * ModelTransform * MeshMatrix);
}

GridMesh2D::~GridMesh2D()
{
}

void GridMesh2D::DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout)
{
	Mesh::DrawLine(commandBuffer, descriptorSet, shaderPipelineLayout);
}

void GridMesh2D::Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(DeltaTime, GameObjectMatrix, ModelMatrix);
}

void GridMesh2D::Destroy()
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

void GridMesh2D::SetMeshPosition(float x, float y)
{
	MeshPosition = glm::vec3(x, y, 0.0f);
}

void GridMesh2D::SetMeshRotation(float x, float y)
{
	MeshRotation = glm::vec3(x, y, 0.0f);
}

void GridMesh2D::SetMeshScale(float x, float y)
{
	MeshScale = glm::vec3(x, y, 0.0f);
}

void GridMesh2D::SetMeshPosition(const glm::vec2& position)
{
	MeshPosition = glm::vec3(position.x, position.y, 0.0f);
}

void GridMesh2D::SetMeshRotation(const glm::vec2& rotation)
{
	MeshRotation = glm::vec3(rotation.x, rotation.y, 0.0f);
}

void GridMesh2D::SetMeshScale(const glm::vec2& scale)
{
	MeshScale = glm::vec3(scale.x, scale.y, 0.0f);
}
