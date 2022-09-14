#pragma once
#include "GameObject2D.h"
#include "Mesh2D.h"
#include "GameObjectManager.h"

class SpriteRenderer : public GameObject2D
{
private:
	std::shared_ptr<Mesh> Sprite;

public:
	SpriteRenderer();
	SpriteRenderer(const std::string Name);
	SpriteRenderer(const std::string Name, const glm::vec2& position, uint32_t Depth);
	SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth);
	SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth);

	SpriteRenderer(const std::string Name, std::shared_ptr<Material> material);
	SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, uint32_t Depth);
	SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth);
	SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth);
	SpriteRenderer(nlohmann::json& json);
	virtual ~SpriteRenderer();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);
	std::shared_ptr<Mesh> GetSprite() { return Sprite; }
};

