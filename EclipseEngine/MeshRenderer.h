#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class MeshRenderer : public ComponentRenderer
{
private:
	std::vector<MeshVertex> vertices = {
			{{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
			{{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
			{{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
			{{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
	};

	std::vector<uint32_t> indices = {
 0, 1, 3, 
		1, 2, 3
	};
	std::shared_ptr<Model> model;

public:
	MeshRenderer();
	MeshRenderer(glm::vec3 position);
	MeshRenderer(glm::vec3 position, glm::vec3 rotation);
	MeshRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	MeshRenderer(const std::string& FilePath);
	MeshRenderer(nlohmann::json& json);
	virtual ~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }
};

