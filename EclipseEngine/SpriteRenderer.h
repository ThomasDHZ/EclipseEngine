#pragma once
#include "Component.h"
#include "Mesh3D.h"
#include "Model.h"
class SpriteRenderer : public Component
{
private:
	std::shared_ptr<Mesh> Sprite;

public:
	SpriteRenderer(uint64_t GameObjectID);
	SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~SpriteRenderer();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);
	std::shared_ptr<Mesh> GetSprite() { return Sprite; }

	virtual void to_json(nlohmann::json& json) override
	{
		Component::to_json(json);
	}
};

