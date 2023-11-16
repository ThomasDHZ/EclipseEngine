#pragma once
#include "Mesh.h"
#include "LevelTile.h"

class Level2DLayerRenderer : public Mesh
{
private:
	std::string TileSetPath;

	std::vector<LevelTile> TileList;
	std::vector<Vertex2D> VertexList;
	std::vector<uint32_t> IndexList;

	glm::ivec2 LevelSizeInPixels;
	glm::ivec2 TileSizeInPixels;
	glm::vec2 TileSize = glm::vec2(1.0f);
	glm::ivec2 LevelTileCount;
	glm::ivec2 LevelBounds;
	glm::vec2 TileUVSize;


	void LoadLevelTiles();

public:
	Level2DLayerRenderer();
	Level2DLayerRenderer(std::vector<LevelTile>& tileList, std::shared_ptr<Material> levelMaterial, glm::ivec2 levelBounds, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~Level2DLayerRenderer();

	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
};

