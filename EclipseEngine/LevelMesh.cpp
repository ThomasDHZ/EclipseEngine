#include "LevelMesh.h"

LevelMesh::LevelMesh()
{
}

LevelMesh::LevelMesh(const std::string& SpriteName, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	std::vector<Vertex2D> SpriteVertexList;
	std::vector<uint32_t> SpriteIndexList;

	auto b = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";

	GLTFInstancingDataStruct instance = {};
	std::vector<std::shared_ptr<Material>> instanceMaterialList;

	const std::vector<Vertex2D> TileVertexList =
	{
	  { {0.0f, 0.0f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 1.0f},  {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
	};
	const std::vector<uint32_t> TileIndexList = {
		0, 1, 3,
		1, 2, 3
	};

	for (int x = 0; x < 500; x++)
	{
		for (int y = 0; y < 500; y++)
		{
			GLTFInstanceMeshDataStruct instanceMeshDataStruct = {};
			instanceMeshDataStruct.VertexData = TileVertexList;
			instanceMeshDataStruct.IndiceData = TileIndexList;
			instanceMeshDataStruct.InstancePosition = glm::vec3(float(x * 1.0f), float(y * 1.0f), 0.0f);

			instance.InstanceMeshDataList.emplace_back(instanceMeshDataStruct);
			instance.MaterialList = materialList;
		}
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = SpriteName;
	gltfMaterialList = materialList;

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

	InstancingStartUp(instance);
}

LevelMesh::~LevelMesh()
{
}

void LevelMesh::DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites)
{
	Mesh::DrawLevelLayer(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperites);
}

void LevelMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(GameObjectMatrix, ModelMatrix);
}