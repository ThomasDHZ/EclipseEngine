#include "Level2DRenderer.h"
#include "MeshRendererManager.h"

Level2DRenderer::Level2DRenderer()
{
}

Level2DRenderer::Level2DRenderer(std::shared_ptr<Level2D> level, uint64_t GameObjectID)
{
	//MeshRendererManager::AddMesh(level2D);
}

Level2DRenderer::Level2DRenderer(glm::ivec2 levelTilesSize, glm::ivec2 levelTileCount, std::shared_ptr<Material> levelTileSet, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	level2D = std::make_shared<Level2D>(Level2D(levelTilesSize, levelTileCount, levelTileSet));
	//MeshRendererManager::AddMesh(level2D);
}

Level2DRenderer::~Level2DRenderer()
{
}

void Level2DRenderer::Update(float DeltaTime)
{
	level2D->Update(DeltaTime);
}

void Level2DRenderer::Destroy()
{
	level2D->Destroy();
}
