#pragma once
#include "Mesh3D.h"

class Particle : public Mesh3D
{
private:
	float Gravity;
	float LifeSpan;
	float ElapsedTime = 0.0f;

public:
	Particle();
	~Particle();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Destroy() override;
};

