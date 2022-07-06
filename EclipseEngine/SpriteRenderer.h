#pragma once
#include "ComponentRenderer.h"
#include "Mesh2D.h"

class SpriteRenderer : public ComponentRenderer
{
private:
	std::vector<std::shared_ptr<Mesh>> SpriteList;

public:
	SpriteRenderer(uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~SpriteRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::vector<std::shared_ptr<Mesh>> GetSpriteList() { return SpriteList; }

	virtual void to_json(nlohmann::json& json) override
	{
		ComponentRenderer::to_json(json);
	}
};

