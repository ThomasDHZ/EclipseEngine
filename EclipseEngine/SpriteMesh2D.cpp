#include "SpriteMesh2D.h"

SpriteMesh2D::SpriteMesh2D()
{
}

SpriteMesh2D::SpriteMesh2D(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	std::vector<Vertex3D> SpriteVertexList;
	std::vector<uint32_t> SpriteIndexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	SpriteIndexList = {

		0, 1, 3,
		1, 2, 3

	};

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = SpriteName;

	VertexCount = SpriteVertexList.size();
	IndexCount = SpriteIndexList.size();
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

	VertexBuffer.CreateBuffer(SpriteVertexList.data(), SpriteVertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(SpriteIndexList.data(), SpriteIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshTransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

SpriteMesh2D::~SpriteMesh2D()
{
}

void SpriteMesh2D::DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties)
{
	Mesh::DrawSprite(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperties);
}

void SpriteMesh2D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(GameObjectMatrix, ModelMatrix);
}

void SpriteMesh2D::SetSpritePosition(float x, float y, int drawLayer)
{
	MeshPosition = glm::vec3(x, y, drawLayer);
}

void SpriteMesh2D::SetSpriteRotation(float x, float y)
{
	MeshRotation = glm::vec3(x, y, 0.0f);
}

void SpriteMesh2D::SetSpriteScale(float x, float y)
{
	MeshScale = glm::vec3(x, y, 1.0f);
}

void SpriteMesh2D::SetSpritePosition(const glm::vec2& position, int drawLayer)
{
	MeshPosition = glm::vec3(position.x, position.y, drawLayer);
}

void SpriteMesh2D::SetSpriteRotation(const glm::vec2& rotation)
{
	MeshRotation = glm::vec3(rotation.x, rotation.y, 0.0f);
}

void SpriteMesh2D::SetSpriteScale(const glm::vec2& scale)
{
	MeshScale = glm::vec3(scale.x, scale.y, 1.0f);
}
