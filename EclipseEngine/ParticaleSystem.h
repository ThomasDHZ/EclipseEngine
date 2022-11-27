#pragma once
#include "Particle.h"

class ParticaleSystem
{
private:
	std::vector<Vertex3D> ParticaleVertexList;
	std::vector<uint32_t> ParticaleIndiceList;
	//std::vector<Particle> ParticaleList;

	uint32_t ParticaleCount;
	uint32_t VertexCountPerParticale;

public:
	ParticaleSystem();
	ParticaleSystem(MeshLoader3D& particleLoader, uint32_t particaleCount);
	~ParticaleSystem();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix);
	void Draw(VkCommandBuffer& commandBuffer);
	void Destroy();
};

