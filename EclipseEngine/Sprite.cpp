#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(std::shared_ptr<Material> material, glm::vec2 spriteSize, glm::ivec2 tileSizeInPixels, glm::ivec2 spritePosition, std::vector<glm::ivec2>& animationFrameOffsets, float FrameTime)
{
	SpriteMaterial = material;

	SpriteSize = spriteSize;
	SpriteSizeInPixels = tileSizeInPixels;
	SpritePosition = spritePosition;
	SpriteCellCount = glm::ivec2(SpriteMaterial->GetAlbedoMap()->GetWidth() / SpriteSizeInPixels.x, SpriteMaterial->GetAlbedoMap()->GetHeight() / SpriteSizeInPixels.y);
	SpriteUVSize = glm::vec2(1.0f / (float)SpriteCellCount.x, 1.0f / (float)SpriteCellCount.y);

	Animated = true;
	CurrentFrame = 0;
	CurrentFrameTime = 0;
	FrameHoldTime = FrameTime;
	AnimationFrameOffsets = animationFrameOffsets;


	SpriteVertexList =
	{
	  { {0.0f, SpriteSize.y},  {0.0f,           0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {SpriteSize.x, SpriteSize.y},  {SpriteUVSize.x, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {SpriteSize.x, 0.0f},  {SpriteUVSize.x, SpriteUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} },
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
}

void Sprite::LoadSprite(nlohmann::json& json)
{
	JsonConverter::from_json(json["SpritePosition"], SpritePosition);
	for (int x = 0; x < AnimationFrameOffsets.size(); x++)
	{
		JsonConverter::from_json(json["AnimationFrameOffsets"][x], AnimationFrameOffsets[x]);
	}
}

void Sprite::SaveSprite(nlohmann::json& json)
{
	JsonConverter::to_json(json["SpritePosition"], SpritePosition);
	for (int x = 0; x < AnimationFrameOffsets.size(); x++)
	{
		JsonConverter::to_json(json["AnimationFrameOffsets"][x], AnimationFrameOffsets[x]);
	}
}