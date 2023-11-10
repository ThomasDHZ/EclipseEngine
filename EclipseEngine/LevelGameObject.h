#pragma once
#include "GameObject2D.h"

class LevelGameObject : public GameObject2D
{
private:
public:
	LevelGameObject();
	LevelGameObject(const std::string Name);
	LevelGameObject(const std::string Name, const glm::vec2& position, int drawLayer);
	LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	virtual ~LevelGameObject();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void LoadLevelGameObject2D(const std::string& levelPath);
	void LoadLevelGameObject2D(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::string tileSetPath);
};

