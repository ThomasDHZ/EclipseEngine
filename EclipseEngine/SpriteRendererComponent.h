#pragma once
#include "Component.h"
#include "Mesh3D.h"
#include "Model.h"
class SpriteRendererComponent : public Component
{
private:
	std::shared_ptr<Mesh> Sprite;

public:
	SpriteRendererComponent(uint64_t GameObjectID);
	SpriteRendererComponent(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~SpriteRendererComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);
	std::shared_ptr<Mesh> GetSprite() { return Sprite; }

	virtual void to_json(nlohmann::json& json) override
	{
		Component::to_json(json);
	}
};

