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

public:
	MeshRenderer(uint64_t GameObjectID);
	MeshRenderer(glm::vec3 position, uint64_t GameObjectID);
	MeshRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	MeshRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	MeshRenderer(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;
};

