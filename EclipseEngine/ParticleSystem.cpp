#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(ParticleLoader3D& particleLoader)
{
	BasePosition = glm::vec3(0.0f);
	ParticaleCount = particleLoader.ParticaleCount;
	//ParticaleVertexList.resize(ParticaleCount * 4);

	for (int x = 0; x < ParticaleCount; x++)
	{
		auto particle = std::make_shared<Particle>(Particle(particleLoader));
		ParticaleList.emplace_back(particle);
		MeshRendererManager::AddMesh(ParticaleList.back());

		EmitParticle(particle);
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

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::EmitParticle(std::shared_ptr<Particle> particle)
{
	particle->Alive = true;
	particle->ElapsedTime = 0.0f;
	particle->MeshPosition = glm::vec3(BasePosition.x + RandomNumber::GenerateRandomFloat(0.0f, 1.0f), BasePosition.y + RandomNumber::GenerateRandomFloat(0.0f, 1.0f), BasePosition.z + RandomNumber::GenerateRandomFloat(0.0f, 1.0f));
	particle->Velocity = glm::vec3(RandomNumber::GenerateRandomFloat(0.1f, 0.25f), RandomNumber::GenerateRandomFloat(0.4f, 1.00f), RandomNumber::GenerateRandomFloat(0.1f, 0.25f));
	glm::normalize(particle->Velocity);
}

void ParticleSystem::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	for (auto& particale : ParticaleList)
	{
		if (particale->Alive)
		{
			particale->Update(GameObjectMatrix, ModelMatrix);
		}
		else
		{
			EmitParticle(particale);
		}
	}
	//memcpy(&ParticaleVertexList, &ParticaleList[0], sizeof(Particle) * ParticaleCount);
}

void ParticleSystem::Destroy()
{
	for (auto& particale : ParticaleList)
	{
		particale->Destroy();
	}
}
