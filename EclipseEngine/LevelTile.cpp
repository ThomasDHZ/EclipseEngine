#include "LevelTile.h"
uint32_t LevelTile::TileID = 0;

LevelTile::LevelTile()
{
}

LevelTile::LevelTile(glm::ivec2 tilePositionOffset, glm::ivec2 tileOffset, glm::vec2& tileUVSize)
{
	TilePositionOffset = tilePositionOffset;
	TileOffset = tileOffset;
	TileUVSize = tileUVSize;

	Animated = false;
	CurrentFrame = 0;
	CurrentFrameTime = 0;
	FrameHoldTime = 0;

	TileVertexList =
	{
	  { {0.0f, 1.0f},  {0.0f        , 0.0f        }, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {TileUVSize.x, 0.0f        }, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {TileUVSize.x, TileUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 0.0f},  {0.0f        , TileUVSize.y}, {1.0f, 1.0f, 0.0f, 1.0f}}
	};
	TileIndexList = {
	   0, 1, 3,
	   1, 2, 3
	};
}

LevelTile::LevelTile(glm::ivec2 tilePositionOffset, glm::ivec2 tileOffset, glm::vec2& tileUVSize, std::vector<glm::ivec2>& animationFrameOffsets, float FrameTime)
{
	TilePositionOffset = tilePositionOffset;
	TileOffset = tileOffset;
	TileUVSize = tileUVSize;

	Animated = true;
	CurrentFrame = 0;
	CurrentFrameTime = 0;
	FrameHoldTime = 0;
	AnimationFrameOffsets = animationFrameOffsets;


	TileVertexList =
	{
	  { {0.0f, 1.0f},  {0.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {TileUVSize.x, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {TileUVSize.x, TileUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 0.0f},  {0.0f,  TileUVSize.y}, {1.0f, 1.0f, 0.0f, 1.0f}}
	};
	TileIndexList = {
	   0, 1, 3,
	   1, 2, 3
	};
}

LevelTile::~LevelTile()
{
}

void LevelTile::Update(std::shared_ptr<Timer> timer)
{
	if (Animated)
	{
		CurrentFrameTime += timer->GetTimerDurationMilliseconds();
		while (CurrentFrameTime >= FrameHoldTime)
		{
			CurrentFrame += 1;
			if (CurrentFrame > AnimationFrameOffsets.size() - 1)
			{
				CurrentFrame = 0;
			}
			CurrentFrameTime -= FrameHoldTime;
		}

		CurrentTileUV = glm::vec2(TileUVSize.x * AnimationFrameOffsets[CurrentFrame].x, TileUVSize.y * AnimationFrameOffsets[CurrentFrame].y);
	}
	else
	{
		CurrentTileUV = glm::vec2(TileUVSize.x * TileOffset.x, TileUVSize.y * TileOffset.y);
	}
}

glm::vec2 LevelTile::GetTileUVs()
{
	return glm::vec2(TileUVSize.x * TileOffset.x, TileUVSize.y * TileOffset.y);
}