//#include "ParticleSystem.h"
//
//ParticleSystem::ParticleSystem()
//{
//}
//
//ParticleSystem::ParticleSystem(ParticleLoader3D& particleLoader)
//{
//	BasePosition = particleLoader.particleInfo.BasePosition;
//	VelocityRange = particleLoader.particleInfo.VelocityRange;
//	PositionOffsetRange = particleLoader.particleInfo.PositionOffsetRange;
//	ScaleRange = particleLoader.particleInfo.ScaleRange;
//
//	ParticaleCount = particleLoader.ParticaleCount;
//	//ParticaleVertexList.resize(ParticaleCount * 4);
//
//	for (int x = 0; x < ParticaleCount; x++)
//	{
//		auto particle = std::make_shared<Particle>(Particle(particleLoader));
//		ParticaleList.emplace_back(particle);
//		MeshRendererManager::AddMesh(ParticaleList.back());
//
//		EmitParticle(particle);
//	}
//	//for (int x = 0; x < ParticaleCount; x++)
//	//{
//	//	const uint32_t ParticalIndex = x * 4;
//	//	ParticaleIndiceList.emplace_back(ParticalIndex);
//	//	ParticaleIndiceList.emplace_back(ParticalIndex + 1);
//	//	ParticaleIndiceList.emplace_back(ParticalIndex + 2);
//	//	ParticaleIndiceList.emplace_back(ParticalIndex + 2);
//	//	ParticaleIndiceList.emplace_back(ParticalIndex + 3);
//	//	ParticaleIndiceList.emplace_back(ParticalIndex);
//	//}
//}
//
//ParticleSystem::~ParticleSystem()
//{
//}
//
//void ParticleSystem::EmitParticle(std::shared_ptr<Particle> particle)
//{
//	particle->Alive = true;
//	particle->ElapsedTime = 0.0f;
//	particle->MeshPosition = glm::vec3(BasePosition.x + RandomNumber::GenerateRandomFloat(PositionOffsetRange[0].x, PositionOffsetRange[0].y), 
//									   BasePosition.y + RandomNumber::GenerateRandomFloat(PositionOffsetRange[1].x, PositionOffsetRange[1].y), 
//									   BasePosition.z + RandomNumber::GenerateRandomFloat(PositionOffsetRange[2].x, PositionOffsetRange[2].y));
//	particle->MeshScale = glm::vec3(RandomNumber::GenerateRandomFloat(ScaleRange[0].x, ScaleRange[0].y),
//									RandomNumber::GenerateRandomFloat(ScaleRange[1].x, ScaleRange[1].y),
//								 	RandomNumber::GenerateRandomFloat(ScaleRange[2].x, ScaleRange[2].y));
//	particle->Velocity = glm::vec3(RandomNumber::GenerateRandomFloat(VelocityRange[0].x, VelocityRange[0].y),
//								   RandomNumber::GenerateRandomFloat(VelocityRange[1].x, VelocityRange[1].y),
//								   RandomNumber::GenerateRandomFloat(VelocityRange[2].x, VelocityRange[2].y));
//	glm::normalize(particle->Velocity);
//}
//
//void ParticleSystem::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
//{
//	for (auto& particale : ParticaleList)
//	{
//		if (particale->Alive)
//		{
//			particale->Update(GameObjectMatrix, ModelMatrix);
//		}
//		else
//		{
//			EmitParticle(particale);
//		}
//	}
//	//memcpy(&ParticaleVertexList, &ParticaleList[0], sizeof(Particle) * ParticaleCount);
//}
//
//void ParticleSystem::Destroy()
//{
//	for (auto& particale : ParticaleList)
//	{
//		particale->Destroy();
//	}
//}
