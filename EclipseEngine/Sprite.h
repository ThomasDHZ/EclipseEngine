#pragma once
#include "Mesh.h"
#include "AnimationPlayer2D.h"
class Sprite
{
private:
	std::vector<Vertex2D>	SpriteVertexList;
	std::vector<uint32_t>	SpriteIndexList;

	glm::vec2				SpriteSize;
	glm::ivec2				SpriteCellCount;
	glm::ivec2				SpriteSizeInPixels;
	glm::ivec2				SpritePosition;
	glm::vec2				SpriteUVSize;
	glm::vec2				CurrentSpriteUV;

	bool					DrawSprite = true;
	bool					Animated;
	uint32_t				CurrentFrame;
	float					CurrentFrameTime;
	float					FrameHoldTime;
	std::vector<glm::ivec2> AnimationFrameOffsets;

	std::shared_ptr<Material> SpriteMaterial;

public:
	Sprite();
	Sprite(std::shared_ptr<Material> material, glm::vec2 spriteSize, glm::ivec2 tileSizeInPixels, glm::ivec2 SpritePosition, std::vector<glm::ivec2>& AnimationFrameOffsets, float FrameTime);
	~Sprite();

	void Update(float DeltaTime);
	void LoadSprite(nlohmann::json& json);
	void SaveSprite(nlohmann::json& json);

	glm::vec2 GetCurrentSpriteUV() { return CurrentSpriteUV; }
	glm::vec2 GetSpritePosition() { return SpritePosition; }
	std::vector<Vertex2D> GetSpriteVertexList() { return SpriteVertexList; }
	std::vector<uint32_t> GetSpriteIndexList() { return SpriteIndexList; }
	std::shared_ptr<Material> GetMaterial() { return SpriteMaterial; }
};

