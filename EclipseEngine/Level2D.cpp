#include "Level2D.h"

Level2D::Level2D()
{
}

Level2D::Level2D(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelID)
{
	LevelName = levelName;
	LevelBounds = levelBounds;
	LevelSizeInPixels = glm::ivec2(materialList[0]->AlbedoMap->GetWidth(), materialList[0]->AlbedoMap->GetHeight());
	TileSizeInPixels = tileSizeInPixels;
	LevelTileCount = glm::ivec2(LevelSizeInPixels.x / TileSizeInPixels.x, LevelSizeInPixels.y / TileSizeInPixels.y);
	TileUVSize = glm::vec2(1.0f / (float)LevelTileCount.x, 1.0f / (float)LevelTileCount.y);

	std::vector<glm::ivec2> animation;
	animation.emplace_back(glm::ivec2(1, 0));
	animation.emplace_back(glm::ivec2(2, 0));
	animation.emplace_back(glm::ivec2(3, 0));
	animation.emplace_back(glm::ivec2(4, 0));
	animation.emplace_back(glm::ivec2(5, 0));
	animation.emplace_back(glm::ivec2(6, 0));
	animation.emplace_back(glm::ivec2(7, 0));
	animation.emplace_back(glm::ivec2(8, 0));
	animation.emplace_back(glm::ivec2(9, 0));
	animation.emplace_back(glm::ivec2(10, 0));
	animation.emplace_back(glm::ivec2(11, 0));

	std::vector<LevelTile> TileList;

	TileList.emplace_back(LevelTile(glm::ivec2(6, 12), glm::ivec2(7, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(7, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(8, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(9, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(10, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(11, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(12, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(13, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(14, 12), glm::ivec2(8, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 12), glm::ivec2(9, 0), TileUVSize));

	TileList.emplace_back(LevelTile(glm::ivec2(15, 11), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 10), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 9), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 8), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 7), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 6), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 5), glm::ivec2(2, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 4), glm::ivec2(2, 0), TileUVSize));

	TileList.emplace_back(LevelTile(glm::ivec2(16, 11), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 10), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 9), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 8), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 7), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 6), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 5), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 4), glm::ivec2(3, 0), TileUVSize));

	TileList.emplace_back(LevelTile(glm::ivec2(0, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(1, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(2, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(3, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(4, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(5, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(6, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(7, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(8, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(9, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(10, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(11, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(12, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(13, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(14, 3), glm::ivec2(13, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 3), glm::ivec2(17, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 3), glm::ivec2(3, 0), TileUVSize));

	TileList.emplace_back(LevelTile(glm::ivec2(0, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(1, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(2, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(3, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(4, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(5, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(6, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(7, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(8, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(9, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(10, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(11, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(12, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(13, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(14, 2), glm::ivec2(3, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 2), glm::ivec2(4, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 2), glm::ivec2(3, 0), TileUVSize));

	TileList.emplace_back(LevelTile(glm::ivec2(0, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(1, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(2, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(3, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(4, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(5, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(6, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(7, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(8, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(9, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(10, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(11, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(12, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(13, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(14, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(15, 1), glm::ivec2(6, 0), TileUVSize));
	TileList.emplace_back(LevelTile(glm::ivec2(16, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(3, 1), glm::ivec2(0, 0), TileUVSize, animation, 500));

	DrawLevelLayerMeshList.emplace_back(SpriteLayerMesh(levelName, TileList, materialList, GameObjectMatrix, ModelMatrix, gameObjectID, modelID));
}

Level2D::~Level2D()
{
}

void Level2D::LoadLevel()
{


	
}

void Level2D::Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	for (auto& layerMesh : DrawLevelLayerMeshList)
	{
		layerMesh.Update(DeltaTime, GameObjectMatrix, ModelMatrix);
	}
}

void Level2D::DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer)
{
	for (auto& layerMesh : DrawLevelLayerMeshList)
	{
		layerMesh.DrawLevelLayer(commandBuffer, descriptorSet, shaderPipelineLayout, constBuffer);
	}
}
