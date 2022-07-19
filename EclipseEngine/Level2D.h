#pragma once
#include <vector>
#include <memory>

#include "Mesh2D.h"
#include "Tile.h"

class Level2D
{
private: 
	std::vector<std::shared_ptr<Tile>> TileList;
	std::shared_ptr<Material> LevelTileSet;

	glm::ivec2 LevelTilesSize;
	glm::ivec2 LevelTileCount;

	std::shared_ptr<Mesh2D> LevelMesh;

public:
	Level2D();
	Level2D(glm::ivec2 levelTilesSize, glm::ivec2 levelTileCount, std::shared_ptr<Material> levelTileSet);
	~Level2D();

	virtual void Update(float DeltaTime);
	virtual void Destroy();

	std::shared_ptr<Mesh2D> GetLevelMesh() { return LevelMesh; }
};

