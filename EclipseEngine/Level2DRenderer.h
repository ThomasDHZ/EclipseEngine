#pragma once
#include "Component.h"
#include "Level2D.h"

class Level2DRenderer : public Component
{
private:
	std::shared_ptr<Level2D> level2D;

public:
	Level2DRenderer();
	Level2DRenderer(std::shared_ptr<Level2D> level, uint64_t GameObjectID);
	Level2DRenderer(glm::ivec2 levelTilesSize, glm::ivec2 levelTileCount, std::shared_ptr<Material> levelTileSet, uint64_t GameObjectID);
	virtual ~Level2DRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	virtual void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["componentType"], componentType);
		JsonConverter::to_json(json["componentSubType"], componentSubType);
	}
};

