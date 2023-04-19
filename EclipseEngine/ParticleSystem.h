#pragma once
#include "Particle.h"
#include "RandomNumber.h"

class ParticleSystem
{
private:
	std::vector<Vertex3D> ParticaleVertexList;
	std::vector<uint32_t> ParticaleIndiceList;
	std::vector<std::shared_ptr<Particle>> ParticaleList;

	uint32_t ParticaleCount;
	uint32_t VertexCountPerParticle;

	glm::vec3 BasePosition;
	std::array<glm::vec2, 3> VelocityRange;
	std::array<glm::vec2, 3> PositionOffsetRange;
	std::array<glm::vec2, 3> ScaleRange;

	void EmitParticle(std::shared_ptr<Particle> particle);
public:
	ParticleSystem();
	ParticleSystem(ParticleLoader3D& particleLoader);
	~ParticleSystem();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Destroy();
};

