#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::CreateParticle(ParticleInfo& particle)
{
	Gravity = particle.Gravity;
	LifeSpan = particle.LifeSpan;
	ElapsedTime = 0.0f;
	Alive = true;

	Position = particle.Position;
	Rotation = particle.Rotation;
	Scale = particle.Scale;
	Velocity = particle.Velocity;

	ParticleMaterial = particle.ParticleMaterial;
}

void Particle::Update()
{
	if (Alive)
	{
		Position += Velocity * VulkanRenderer::GetFrameTimeDurationSeconds();
		ElapsedTime += VulkanRenderer::GetFrameTimeDurationSeconds();

		if (LifeSpan < ElapsedTime)
		{
			Alive = false;
		}
	}
}

void Particle::SetVelocity(glm::vec3 velocity)
{
	Velocity = velocity;
}
