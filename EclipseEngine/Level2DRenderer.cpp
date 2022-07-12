#include "Level2DRenderer.h"

Level2DRenderer::Level2DRenderer()
{
}

Level2DRenderer::Level2DRenderer(glm::ivec2 levelTilesSize, glm::ivec2 levelTileCount, std::shared_ptr<Material> levelTileSet, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	LevelTilesSize = levelTilesSize;
	LevelTileCount = levelTileCount;
	LevelTileSet = levelTileSet;

	//std::vector<Sprite> VertexList;
	std::vector<uint32_t> IndexList;
}

Level2DRenderer::~Level2DRenderer()
{
}

void Level2DRenderer::Update(float DeltaTime)
{
}

void Level2DRenderer::Destroy()
{
}
