#include "ParticaleSystem.h"

ParticaleSystem::ParticaleSystem()
{
}

ParticaleSystem::ParticaleSystem(ParticleLoader3D& particleLoader)
{
	ParticaleCount = particleLoader.ParticaleCount;
	//ParticaleVertexList.resize(ParticaleCount * 4);

	for (int x = 0; x < ParticaleCount; x++)
	{
		auto particle = std::make_shared<Particle>(Particle(particleLoader));
		particle->MeshPosition = glm::vec3(std::rand() % 10, std::rand() % 10, std::rand() % 10);
		//particle->MeshRotation = glm::vec3(std::rand() % 360, std::rand() % 360, std::rand() % 360);

		ParticaleList.emplace_back(particle);
		MeshRendererManager::AddMesh(ParticaleList.back());
	}
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
	for (auto& particale : ParticaleList)
	{
		particale->Update(GameObjectMatrix, ModelMatrix);
	}
	//memcpy(&ParticaleVertexList, &ParticaleList[0], sizeof(Particle) * ParticaleCount);
}

void ParticaleSystem::Destroy()
{
	for (auto& particale : ParticaleList)
	{
		particale->Destroy();
	}
}
