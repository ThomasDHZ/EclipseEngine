#pragma once
#include "Mesh3D.h"

struct ParticleInfo
{
	float Gravity;
	float LifeSpan;
	glm::vec3 BasePosition;
	std::array<glm::vec2, 3> VelocityRange;
	std::array<glm::vec2, 3> PositionOffsetRange;
	std::array<glm::vec2, 3> ScaleRange;
};

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
	//std::shared_ptr<Material> MaterialPtr = MaterialManager::GetDefaultMaterial();

	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	ParticleInfo particleInfo;
};

class Particle : public Mesh3D
{
private:
	float Gravity;
	float LifeSpan;

public:
	Particle();
	Particle(ParticleLoader3D& particaleLoader);
	~Particle();

	bool  Alive;
	float ElapsedTime = 0.0f;
	glm::vec3 Velocity = glm::vec3(0.0f);

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
};

