#pragma once
#include "Mesh.h"

class Mesh2D : public Mesh
{
private:
	std::vector<MeshVertex> vertices = {
			{{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
			{{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
			{{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
			{{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	int MeshDepth = 0;
	glm::vec2 MeshPosition = glm::vec3(0.0f);
	glm::vec2 MeshRotation = glm::vec3(0.0f);
	glm::vec2 MeshScale = glm::vec3(1.0f);
	glm::mat4 MeshTransformMatrix = glm::mat4(1.0f);

public:
	Mesh2D();
	Mesh2D(uint64_t parentGameObjectID);
	Mesh2D(std::shared_ptr<Material> SpriteMaterial, uint64_t parentGameObjectID);
	~Mesh2D();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	void SetPosition(float x, float y);
	void SetPosition(glm::vec2 position);
	void SetDepth(float Depth);
	void SetRotation(float x, float y);
	void SetRotation(glm::vec2 rotation);
	void SetScale(float x, float y);
	void SetScale(glm::vec2 Scale);

	float GetDepth() { return MeshDepth; }
	glm::vec2 GetPosition() { return MeshPosition; }
	glm::vec2 GetRotation() { return MeshRotation; }
	glm::vec2 GetScale() { return MeshScale; }

	//glm::vec2* GetMeshPositionPtr() { return &MeshPosition; }
	//glm::vec2* GetMeshRotationPtr() { return &MeshRotation; }
	//glm::vec2* GetMeshScalePtr() { return &MeshScale; };
};

