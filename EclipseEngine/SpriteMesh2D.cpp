#include "SpriteMesh2D.h"
#include "GLTFSceneManager.h"

SpriteMesh2D::SpriteMesh2D()
{
}

SpriteMesh2D::SpriteMesh2D(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	std::vector<Vertex2D> SpriteVertexList =
	{
	  { {0.0f, 0.0f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 1.0f},  {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
	};
	std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 3,
	   1, 2, 3
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

SpriteMesh2D::~SpriteMesh2D()
{
}

void SpriteMesh2D::DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites)
{
	Mesh::DrawSprite(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperites);
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
