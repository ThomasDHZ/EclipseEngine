#pragma once
#include "ComponentRenderer.h"
#include "Mesh.h"

class SpriteRenderer : public ComponentRenderer
{
private:
	std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	std::vector<uint32_t> indices = {
0, 1, 2, 2, 3, 0
	};


public:
	SpriteRenderer();
	SpriteRenderer(nlohmann::json& json);
	virtual ~SpriteRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;


	virtual nlohmann::json ToJson() override
	{
		nlohmann::json json = Component::ToJson();

		return json;
	}
};

