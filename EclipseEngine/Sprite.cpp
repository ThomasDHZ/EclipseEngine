#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(std::shared_ptr<Material> material, glm::ivec2 tilePositionOffset, glm::ivec2 tileOffset, glm::vec2& tileUVSize, bool drawTile)
{
	SpriteMaterial = material;

	SpritePositionOffset = tilePositionOffset;
	SpriteOffset = tileOffset;
	SpriteUVSize = tileUVSize;

	DrawSprite = drawTile;
	Animated = false;
	CurrentFrame = 0;
	CurrentFrameTime = 0;
	FrameHoldTime = 0;

	SpriteVertexList =
	{
	  { {0.0f, 1.0f},  {0.0f          , 0.0f        }, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {SpriteUVSize.x, 0.0f        }, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {SpriteUVSize.x, SpriteUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 0.0f},  {0.0f          , SpriteUVSize.y}, {1.0f, 1.0f, 0.0f, 1.0f}}
	};
	SpriteIndexList = {
	   0, 1, 3,
	   1, 2, 3
	};
}

Sprite::Sprite(std::shared_ptr<Material> material, glm::ivec2 tilePositionOffset, glm::ivec2 tileOffset, glm::vec2& tileUVSize, std::vector<glm::ivec2>& animationFrameOffsets, float FrameTime)
{
	SpriteMaterial = material;

	SpritePositionOffset = tilePositionOffset;
	SpriteOffset = tileOffset;
	SpriteUVSize = tileUVSize;

	Animated = true;
	CurrentFrame = 0;
	CurrentFrameTime = 0;
	FrameHoldTime = FrameTime;
	AnimationFrameOffsets = animationFrameOffsets;


	SpriteVertexList =
	{
	  { {0.0f, 1.0f},  {0.0f,           0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {SpriteUVSize.x, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {SpriteUVSize.x, SpriteUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 0.0f},  {0.0f,           SpriteUVSize.y}, {1.0f, 1.0f, 0.0f, 1.0f}}
	};
	SpriteIndexList = {
	   0, 1, 3,
	   1, 2, 3
	};
}

Sprite::~Sprite()
{
}

void Sprite::Update(float DeltaTime)
{
	if (DrawSprite)
	{
		if (Animated)
		{
			CurrentFrameTime += DeltaTime;
			if (CurrentFrameTime >= FrameHoldTime)
			{
				CurrentFrame += 1;
				if (CurrentFrame > AnimationFrameOffsets.size() - 1)
				{
					CurrentFrame = 0;
				}
				CurrentFrameTime = 0.0f;
			}

			CurrentSpriteUV = glm::vec2(SpriteUVSize.x * AnimationFrameOffsets[CurrentFrame].x, SpriteUVSize.y * AnimationFrameOffsets[CurrentFrame].y);
		}
		else
		{
			CurrentSpriteUV = glm::vec2(SpriteUVSize.x * SpriteOffset.x, SpriteUVSize.y * SpriteOffset.y);
		}
	}
}

void Sprite::LoadSprite(nlohmann::json& json)
{
	JsonConverter::from_json(json["SpritePositionOffset"], SpritePositionOffset);
	JsonConverter::from_json(json["SpriteOffset"], SpriteOffset);
	for (int x = 0; x < AnimationFrameOffsets.size(); x++)
	{
		JsonConverter::from_json(json["AnimationFrameOffsets"][x], AnimationFrameOffsets[x]);
	}
}

void Sprite::SaveSprite(nlohmann::json& json)
{
	JsonConverter::to_json(json["SpritePositionOffset"], SpritePositionOffset);
	JsonConverter::to_json(json["SpriteOffset"], SpriteOffset);
	for (int x = 0; x < AnimationFrameOffsets.size(); x++)
	{
		JsonConverter::to_json(json["AnimationFrameOffsets"][x], AnimationFrameOffsets[x]);
	}
}

glm::vec2 Sprite::GetSpriteUVs()
{
	return glm::vec2(SpriteUVSize.x * SpriteOffset.x, SpriteUVSize.y * SpriteOffset.y);
}