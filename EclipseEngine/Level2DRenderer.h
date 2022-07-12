#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class Level2DRenderer : public Component
{
private:
	std::vector<Mesh> LevelTileList;
	glm::ivec2 LevelTilesSize;
	glm::ivec2 LevelTileCount;
	std::shared_ptr<Material> LevelTileSet;

public:
	Level2DRenderer();
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

