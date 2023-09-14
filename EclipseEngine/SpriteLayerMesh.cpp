#include "SpriteLayerMesh.h"
#include "RandomNumber.h"
#include <glm/ext/matrix_transform.hpp>

SpriteLayerMesh::SpriteLayerMesh()
{
}

SpriteLayerMesh::SpriteLayerMesh(const std::string& LevelName, glm::ivec2 levelBounds, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	GLTFInstancingDataStruct instance = {};
	std::vector<std::shared_ptr<Material>> instanceMaterialList;

	glm::ivec2 TileOffset = glm::ivec2(23, 0);
	glm::vec2 TileUVSize = glm::vec2(0.02439f, 1.0f);

	TileList.emplace_back(LevelTile(glm::ivec2(0, 0), glm::ivec2(23, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(1, 0), glm::ivec2(0, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(2, 0), glm::ivec2(1, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(0, 1), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(1, 1), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(2, 1), glm::ivec2(4, 0), TileUVSize));

	for (auto& tile : TileList)
	{
		GLTFInstanceMeshDataStruct instanceMeshDataStruct = {};
		instanceMeshDataStruct.VertexData = tile.GetTileVertexList();
		instanceMeshDataStruct.IndiceData = tile.GetTileIndexList();
		instanceMeshDataStruct.InstancePosition = glm::vec3(float(tile.GetTilePositionOffset().x * 1.0f), float(tile.GetTilePositionOffset().y * 1.0f), 0.0f);

		instance.InstanceMeshDataList.emplace_back(instanceMeshDataStruct);
		instance.UVOffset.emplace_back(glm::vec2(0.0f, 0.0f));
		instance.MaterialList = materialList;
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = LevelName;
	gltfMaterialList = materialList;

	VertexCount = TileList[0].GetTileVertexList().size();
	IndexCount = TileList[0].GetTileIndexList().size();
	BoneCount = 0;
	TriangleCount = 0;
	InstanceCount = instance.InstanceMeshDataList.size();

	GameObjectTransform = GameObjectMatrix;
	ModelTransform = ModelMatrix;
	MeshTransform = glm::mat4(1.0f);

	MeshPosition = glm::vec3(0.0f);
	MeshRotation = glm::vec3(0.0f);
	MeshScale = glm::vec3(1.0f);

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(TileList[0].GetTileVertexList().data(), TileList[0].GetTileVertexList().size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(TileList[0].GetTileIndexList().data(), TileList[0].GetTileIndexList().size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer = VulkanBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	Instancing2DStartUp(instance);
}

SpriteLayerMesh::~SpriteLayerMesh()
{
}

void SpriteLayerMesh::Instancing2DStartUp(GLTFInstancingDataStruct& instanceData)
{
	InstanceCount = instanceData.InstanceMeshDataList.size();
	InstanceData = instanceData;
	if (InstanceCount > 0)
	{
		for (int x = 0; x < instanceData.InstanceMeshDataList.size(); x++)
		{
			InstancedVertexData2D instanceVertexData;

			glm::mat4 TransformMatrix = glm::mat4(1.0f);
			TransformMatrix = glm::translate(TransformMatrix, glm::vec3(instanceData.InstanceMeshDataList[x].InstancePosition.x, instanceData.InstanceMeshDataList[x].InstancePosition.y, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instanceData.InstanceMeshDataList[x].InstanceRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instanceData.InstanceMeshDataList[x].InstanceRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::scale(TransformMatrix, glm::vec3(instanceData.InstanceMeshDataList[x].InstanceScale.x, instanceData.InstanceMeshDataList[x].InstanceScale.y, 0.0f));

			const auto matID = rand() % instanceData.MaterialList.size();

			instanceVertexData.InstanceModel = TransformMatrix;
			instanceVertexData.UVOffset = instanceData.UVOffset[x];
			instanceVertexData.MaterialID = instanceData.MaterialList[matID]->GetMaterialBufferIndex();
			instanceVertexData.FlipSprite = glm::vec2(0.0f, 0.0f);
			InstancedVertex2DDataList.emplace_back(instanceVertexData);
		}

		InstanceBuffer.CreateBuffer(InstancedVertex2DDataList.data(), InstancedVertex2DDataList.size() * sizeof(InstancedVertexData2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

void SpriteLayerMesh::DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites)
{
	Mesh::DrawLevelLayer(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperites);
}

void SpriteLayerMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(GameObjectMatrix, ModelMatrix);

	if (InstanceCount > 0)
	{
		for (int x = 0; x < InstancedVertex2DDataList.size(); x++)
		{
			//TileList[x].Update(timer);
			InstancedVertex2DDataList[x].UVOffset = TileList[x].GetTileUVs();
		}
		InstanceBuffer.UpdateBufferMemory(InstancedVertex2DDataList.data(), InstancedVertex2DDataList.size() * sizeof(InstancedVertexData2D));
	}
}