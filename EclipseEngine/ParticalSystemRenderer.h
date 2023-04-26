//#pragma once
//#include "GameObject3D.h"
//#include "Mesh3D.h"
//#include "ParticleSystem.h"
//
//class ParticalSystemRenderer : public GameObject3D
//{
//private:
//	std::shared_ptr<ParticleSystem> particaleSystem;
//
//public:
//	ParticalSystemRenderer();
//	ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D);
//	ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position);
//	ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position, const glm::vec3& rotation);
//	ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
//	~ParticalSystemRenderer();
//
//	void Update(float DeltaTime) override;
//	void Destroy() override;
//
//	std::shared_ptr<ParticleSystem> GetParticaleSystem() { return particaleSystem; }
//};