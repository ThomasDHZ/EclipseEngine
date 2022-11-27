#include "ParticaleSystem.h"

ParticaleSystem::ParticaleSystem()
{
}

ParticaleSystem::ParticaleSystem(MeshLoader3D& particleLoader, uint32_t particaleCount)
{
	ParticaleCount = particaleCount;
	//ParticaleVertexList.resize(ParticaleCount * 4);

	//for (int x = 0; x < ParticaleCount; x++)
	//{
	//	//ParticaleList.emplace_back(Particle(particleLoader));
	//}
	//for (int x = 0; x < ParticaleCount; x++)
	//{
	//	const uint32_t ParticalIndex = x * 4;
	//	ParticaleIndiceList.emplace_back(ParticalIndex);
	//	ParticaleIndiceList.emplace_back(ParticalIndex + 1);
	//	ParticaleIndiceList.emplace_back(ParticalIndex + 2);
	//	ParticaleIndiceList.emplace_back(ParticalIndex + 2);
	//	ParticaleIndiceList.emplace_back(ParticalIndex + 3);
	//	ParticaleIndiceList.emplace_back(ParticalIndex);
	//}
}

ParticaleSystem::~ParticaleSystem()
{
}

void ParticaleSystem::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	//for (auto& particale : ParticaleList)
	//{
	//	particale.Update(GameObjectMatrix, ModelMatrix);
	//}
	//memcpy(&ParticaleVertexList, &ParticaleList[0], sizeof(Particle) * ParticaleCount);
}

void ParticaleSystem::Draw(VkCommandBuffer& commandBuffer)
{
	//for (auto& particale : ParticaleList)
	//{
	//	particale.Draw(commandBuffer);
	//}
}

void ParticaleSystem::Destroy()
{
	//for (auto& particale : ParticaleList)
	//{
	//	particale.Destroy();
	//}
}
