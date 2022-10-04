#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh3D::Update(GameObjectMatrix, ModelMatrix);
}

void Particle::Destroy()
{
	Mesh3D::Destroy();
}
