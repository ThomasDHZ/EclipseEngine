#pragma once
#include "Particle.h"


class ParticaleSystem
{
private:
	std::vector<Vertex3D> ParticaleVertexList;
	std::vector<uint32_t> ParticaleIndiceList;
	std::vector<std::shared_ptr<Particle>> ParticaleList;

	uint32_t ParticaleCount;
	uint32_t VertexCountPerParticale;

public:
	ParticaleSystem();
	ParticaleSystem(ParticleLoader3D& particleLoader);
	~ParticaleSystem();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Destroy();
};

