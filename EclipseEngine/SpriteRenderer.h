#pragma once
#include "Component.h"
#include "Mesh3D.h"
#include "Model.h"
class SpriteRenderer : public Component
{
private:
	std::shared_ptr<Mesh> Sprite;
	std::shared_ptr<Model> model;
	std::vector<MeshVertex> vertices = {
		{{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
		{{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
		{{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
		{{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};
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
	std::shared_ptr<Model> GetModel() { return model; }
	virtual void to_json(nlohmann::json& json) override
	{
		Component::to_json(json);
	}
};

