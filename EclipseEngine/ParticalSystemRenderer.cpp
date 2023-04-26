//#include "ParticalSystemRenderer.h"
//
//ParticalSystemRenderer::ParticalSystemRenderer()
//{
//}
//
//ParticalSystemRenderer::ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D) : GameObject3D(Name, kMeshRenderer)
//{
//	particaleSystem = std::make_shared<ParticleSystem>(ParticleSystem(particleLoader3D));
//}
//
//ParticalSystemRenderer::ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position) : GameObject3D(Name, kMeshRenderer, position)
//{
//	particaleSystem = std::make_shared<ParticleSystem>(ParticleSystem(particleLoader3D));
//}
//
//ParticalSystemRenderer::ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kMeshRenderer, position, rotation)
//{
//	particaleSystem = std::make_shared<ParticleSystem>(ParticleSystem(particleLoader3D));
//}
//
//ParticalSystemRenderer::ParticalSystemRenderer(const std::string Name, ParticleLoader3D& particleLoader3D, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kMeshRenderer, position, rotation, scale)
//{
//	particaleSystem = std::make_shared<ParticleSystem>(ParticleSystem(particleLoader3D));
//}
//
//ParticalSystemRenderer::~ParticalSystemRenderer()
//{
//}
//
//void ParticalSystemRenderer::Update(float DeltaTime)
//{
//	GameObject::Update(DeltaTime);
//	particaleSystem->Update(GameObjectTransform, glm::mat4(1.0f));
//}
//
//void ParticalSystemRenderer::Destroy()
//{
//	particaleSystem->Destroy();
//}
