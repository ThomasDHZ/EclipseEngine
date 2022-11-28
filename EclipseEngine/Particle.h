#pragma once
#include "Mesh3D.h"

struct ParticleLoader3D
{
	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	uint32_t ParticaleCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	std::vector<Vertex3D> VerticeList;
	std::vector<uint32_t> IndexList;
	std::shared_ptr<Material> MaterialPtr = MaterialManager::GetDefaultMaterial();

	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;
};

class Particle : public Mesh3D
{
private:
	//const std::vector<Vertex3D> vertices = {
	//	{{-0.5f, -0.5f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
	//	{{0.5f, -0.5f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
	//	{{0.5f, 0.5f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
	//	{{-0.5f, 0.5f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
	//};

	//const std::vector<uint32_t> indiceList = { 0, 1, 2, 2, 3, 0 };

	float Gravity;
	float LifeSpan;
	float ElapsedTime = 0.0f;

public:
	Particle();
	Particle(ParticleLoader3D& particaleLoader);
	~Particle();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
};

