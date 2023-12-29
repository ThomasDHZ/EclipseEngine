#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(ParticleSystemInfo& particleSystemInfo)
{
	ParticleCount = particleSystemInfo.ParticaleCount;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	ParticleVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	ParticleVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	ParticleVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	ParticleVertexList.emplace_back(vertex4);

	ParticleIndiceList = {
		0, 1, 2, 2, 3, 0
	};

	ParticleList.resize(ParticleCount, std::make_shared<Particle>(Particle()));
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::EmitParticle(std::shared_ptr<Particle> particle)
{
	ParticleInfo particleInfo;
	particleInfo.Position = glm::vec3(BasePosition.x + RandomNumber::GenerateRandomFloat(PositionOffsetRange[0].x, PositionOffsetRange[0].y),
									   BasePosition.y + RandomNumber::GenerateRandomFloat(PositionOffsetRange[1].x, PositionOffsetRange[1].y), 
									   BasePosition.z + RandomNumber::GenerateRandomFloat(PositionOffsetRange[2].x, PositionOffsetRange[2].y));
	particleInfo.Scale = glm::vec3(RandomNumber::GenerateRandomFloat(ScaleRange[0].x, ScaleRange[0].y),
									RandomNumber::GenerateRandomFloat(ScaleRange[1].x, ScaleRange[1].y),
								 	RandomNumber::GenerateRandomFloat(ScaleRange[2].x, ScaleRange[2].y));
	particleInfo.Velocity = glm::vec3(RandomNumber::GenerateRandomFloat(VelocityRange[0].x, VelocityRange[0].y),
								   RandomNumber::GenerateRandomFloat(VelocityRange[1].x, VelocityRange[1].y),
								   RandomNumber::GenerateRandomFloat(VelocityRange[2].x, VelocityRange[2].y));
	glm::normalize(particleInfo.Velocity);
}

void ParticleSystem::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	for (auto& particale : ParticleList)
	{
		if (particale->IsParticleAlive())
		{
			particale->Update();
		}
		else
		{
			EmitParticle(particale);
		}
	}
	memcpy(&ParticleVertexList, &ParticleList[0], sizeof(Particle) * ParticleCount);
}
