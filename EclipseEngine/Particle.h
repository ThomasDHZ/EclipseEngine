#pragma once
#include <memory>
#include "Material.h"

struct ParticleInfo
{
	float Gravity;
	float LifeSpan;
	std::shared_ptr<Material>  ParticleMaterial;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Velocity = glm::vec3(0.0f);
};

class Particle
{
protected:
	float Gravity;
	float LifeSpan;
	float ElapsedTime = 0.0f;
	bool  Alive;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Velocity = glm::vec3(0.0f);

	std::shared_ptr<Material> ParticleMaterial;
public:
	Particle();
	~Particle();

	void CreateParticle(ParticleInfo& particle);
	void Update();

	void SetVelocity(glm::vec3 velocity);

	bool IsParticleAlive() { return Alive; }
};

