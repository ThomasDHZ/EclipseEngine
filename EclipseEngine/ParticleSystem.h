#pragma once
#include "Mesh.h"
#include "Particle.h"
#include "RandomNumber.h"
#include <memory>

struct ParticleSystemInfo
{
	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	uint32_t ParticaleCount = 0;

	glm::vec3 BasePosition = glm::vec3(0.0f);

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	std::array<glm::vec2, 3> VelocityRange;
	std::array<glm::vec2, 3> PositionOffsetRange;
	std::array<glm::vec2, 3> ScaleRange;
};

class ParticleSystem : public Mesh
{
private:
	uint32_t ParticleCount;
	glm::vec3 BasePosition = glm::vec3(0.0f);

	std::vector<Vertex3D> ParticleVertexList;
	std::vector<uint32_t> ParticleIndiceList;
	std::vector<std::shared_ptr<Particle>> ParticleList;

	std::array<glm::vec2, 3> VelocityRange;
	std::array<glm::vec2, 3> PositionOffsetRange;
	std::array<glm::vec2, 3> ScaleRange;

	void EmitParticle(std::shared_ptr<Particle> particle);

public:
	ParticleSystem();
	ParticleSystem(ParticleSystemInfo& particleSystemInfo);
	~ParticleSystem();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
};

