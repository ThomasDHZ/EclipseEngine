#pragma once
#include "Component.h"
#include "Mesh2D.h"

class SpriteRenderer : public Component
{
private:
	std::shared_ptr<Mesh> Sprite;

public:
	SpriteRenderer(uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	SpriteRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~SpriteRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);
	std::shared_ptr<Mesh> GetSprite() { return Sprite; }

	virtual void to_json(nlohmann::json& json) override
	{
		Component::to_json(json);
	}
};

