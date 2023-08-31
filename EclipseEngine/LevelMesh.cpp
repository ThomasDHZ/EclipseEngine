#include "LevelMesh.h"

LevelMesh::LevelMesh()
{
}

LevelMesh::LevelMesh(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	std::vector<Vertex2D> SpriteVertexList;
	std::vector<uint32_t> SpriteIndexList;

	Vertex2D vertex;
	vertex.Position = { -0.5f, -0.5f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex2D vertex2;
	vertex2.Position = { 0.5f, -0.5f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex2D vertex3;
	vertex3.Position = { 0.5f,  0.5f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex2D vertex4;
	vertex4.Position = { -0.5f,  0.5f };
	vertex4.UV = { 1.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	SpriteIndexList = {
		0, 1, 2, 2, 3, 0
	};

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = SpriteName;
	gltfMaterialList.emplace_back(material);

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

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(SpriteVertexList.data(), SpriteVertexList.size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(SpriteIndexList.data(), SpriteIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer = VulkanBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

LevelMesh::~LevelMesh()
{
}

void LevelMesh::DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites)
{
	Mesh::DrawSprite(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperites);
}

void LevelMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(GameObjectMatrix, ModelMatrix);
}