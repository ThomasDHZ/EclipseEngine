#include "Level2DLayerRenderer.h"
#include <glm/ext/matrix_transform.hpp>

Level2DLayerRenderer::Level2DLayerRenderer()
{
}

Level2DLayerRenderer::Level2DLayerRenderer(std::vector<LevelTile>& tileList, std::shared_ptr<Material> levelMaterial, glm::ivec2 levelBounds, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	TileList = tileList;
	LevelBounds = levelBounds;

	LoadLevelTiles();

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = "LevelName";
	gltfMaterialList.emplace_back(levelMaterial);

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	BoneCount = 0;
	TriangleCount = 0;
	InstanceCount = 0;

	GameObjectTransform = GameObjectMatrix;
	ModelTransform = ModelMatrix;
	MeshTransform = glm::mat4(1.0f);

	MeshPosition = glm::vec3(0.0f);
	MeshRotation = glm::vec3(0.0f);
	MeshScale = glm::vec3(1.0f);

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer = VulkanBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectTransform * ModelTransform * MeshMatrix);
}

Level2DLayerRenderer::~Level2DLayerRenderer()
{
}

void Level2DLayerRenderer::Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(DeltaTime, GameObjectMatrix, ModelMatrix);
}

void Level2DLayerRenderer::LoadLevelTiles()
{
	for (unsigned int x = 0; x < LevelBounds.x; x++)
	{
		for (unsigned int y = 0; y < LevelBounds.y; y++)
		{
			LevelTile tile = TileList[(y * LevelBounds.x) + x];
			const float LefttSideUV = tile.GetTileUVSize().x * tile.GetTileOffset().x;
			const float RightSideUV = (tile.GetTileUVSize().x * tile.GetTileOffset().x) + tile.GetTileUVSize().x;
			const float TopSideUV = tile.GetTileUVSize().y * tile.GetTileOffset().y;
			const float BottomSideUV = (tile.GetTileUVSize().y * tile.GetTileOffset().y) + tile.GetTileUVSize().y;

			const unsigned int VertexCount = VertexList.size();
			const Vertex2D BottomLeftVertex =  { { x * TileSize.x,                 y * TileSize.y},                {LefttSideUV, BottomSideUV}, {1.0f, 0.0f, 0.0f, 1.0f} };
			const Vertex2D BottomRightVertex = { {(x * TileSize.x) + TileSize.x,   y * TileSize.y},                {RightSideUV, BottomSideUV}, {0.0f, 1.0f, 0.0f, 1.0f} };
			const Vertex2D TopRightVertex =    { {(x * TileSize.x) + TileSize.x,  (y * TileSize.y) + TileSize.y},  {RightSideUV, TopSideUV   }, {0.0f, 0.0f, 1.0f, 1.0f} };
			const Vertex2D TopLeftVertex =     { { x * TileSize.x,                (y * TileSize.y) + TileSize.y},  {LefttSideUV, TopSideUV   }, {1.0f, 1.0f, 0.0f, 1.0f} };

			VertexList.emplace_back(BottomLeftVertex);
			VertexList.emplace_back(BottomRightVertex);
			VertexList.emplace_back(TopRightVertex);
			VertexList.emplace_back(TopLeftVertex);

			IndexList.emplace_back(VertexCount);
			IndexList.emplace_back(VertexCount + 1);
			IndexList.emplace_back(VertexCount + 2);
			IndexList.emplace_back(VertexCount + 2);
			IndexList.emplace_back(VertexCount + 3);
			IndexList.emplace_back(VertexCount);
		}
	}
}
