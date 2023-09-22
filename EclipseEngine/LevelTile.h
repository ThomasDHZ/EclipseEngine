#pragma once
#include "Mesh.h"
#include "AnimationPlayer2D.h"

struct RotatingPaletteAnimation
{
	glm::ivec2 PixelOffset;
	std::vector<glm::vec4> ColorList;
	float ColorSwitcherTimer;
};

class LevelTile
{
private:
	static uint32_t			TileID;

	std::vector<Vertex2D>	TileVertexList;
	std::vector<uint32_t>	TileIndexList;

	glm::ivec2				TilePositionOffset;
	glm::ivec2				TileOffset;
	glm::vec2				TileUVSize;
	glm::vec2				CurrentTileUV;
	
	bool					Animated;
	uint32_t				CurrentFrame;
	float					CurrentFrameTime;
	float					FrameHoldTime;
	std::vector<glm::ivec2> AnimationFrameOffsets;

public:
	LevelTile();
	LevelTile(glm::ivec2 TilePositionOffset, glm::ivec2 TileOffset, glm::vec2& TileUVSize);
	LevelTile(glm::ivec2 TilePositionOffset, glm::ivec2 TileOffset, glm::vec2& TileUVSize, std::vector<glm::ivec2>& AnimationFrameOffsets, float FrameTime);
	~LevelTile();

	void Update(float DeltaTime);
	glm::vec2 GetTileUVs();

	glm::vec2 GetCurrentTileUV() { return CurrentTileUV; }
	glm::vec2 GetTilePositionOffset() { return TilePositionOffset; }
	std::vector<Vertex2D> GetTileVertexList() { return TileVertexList; }
	std::vector<uint32_t> GetTileIndexList() { return TileIndexList; }
};

