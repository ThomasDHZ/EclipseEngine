#pragma once
#include "Mesh3D.h"

class Particle : public Mesh3D
{
private:
	//glm::vec3 Position = glm::vec3(0.0f);
	//float PositionPadding = 0.0f;
	//glm::vec3 Normal = glm::vec3(0.0f);
	//float NormalPadding = 0.0f;
	//glm::vec2 UV = glm::vec2(0.0f);
	//glm::vec2 UVPadding = glm::vec2(0.0f);
	//glm::vec3 Tangant = glm::vec3(0.0f);
	//float TangantPadding = 0.0f;
	//glm::vec3 BiTangant = glm::vec3(0.0f);
	//float BiTangentPadding = 0.0f;
	//glm::vec3 Color = glm::vec3(1.0f);
	//float ColorPadding = 0.0f;

	//const std::vector<Vertex3D> ParticleVertexList = {
	//	{ {-0.5f, -0.5f, 0.0f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
	//	{ { 0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
	//	{ { 0.5f,  0.5f, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
	//	{ {-0.5f,  0.5f, 0.0f},  {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} }
	//};

	//const std::vector<Vertex3D> ParticleIndexList = {
	//	0, 1, 2, 2, 3, 0
	//};

	float Gravity;
	float LifeSpan;
	float ElapsedTime = 0.0f;

public:
	Particle();
	~Particle();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Destroy() override;
};

