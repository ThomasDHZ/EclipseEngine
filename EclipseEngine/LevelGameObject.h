#pragma once
#include "GameObject2D.h"

class LevelGameObject : public GameObject2D
{
private:
	const std::vector<Vertex2D> SpriteVertexList =
	{
	  { {0.0f, 0.0f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { {1.0f, 0.0f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { {1.0f, 1.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {0.0f, 1.0f},  {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
	};
	const std::vector<uint32_t> SpriteIndexList = {
		0, 1, 3,
		1, 2, 3
	};

public:
	LevelGameObject();
	LevelGameObject(const std::string Name);
	LevelGameObject(const std::string Name, const glm::vec2& position, int drawLayer);
	LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	virtual ~LevelGameObject();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void LoadSpriteGameObject2D(const std::string Name, std::shared_ptr<Material> material);
};

