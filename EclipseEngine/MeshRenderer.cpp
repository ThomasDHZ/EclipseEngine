//#include "MeshRenderer.h"
//MeshRenderer::MeshRenderer()
//{
//}
//
//MeshRenderer::MeshRenderer(const std::string Name, MeshLoader3D& meshLoader) : GameObject3D(Name, kMeshRenderer)
//{ 
//	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
//	MeshRendererManager::AddMesh(std::make_shared<Mesh3D>(Mesh3D(meshLoader)));
//}
//
//MeshRenderer::MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position) : GameObject3D(Name, kMeshRenderer, position)
//{
//	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
//	MeshRendererManager::AddMesh(mesh);
//}
//
//MeshRenderer::MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kMeshRenderer, position, rotation)
//{
//	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
//	MeshRendererManager::AddMesh(mesh);
//}
//
//MeshRenderer::MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kMeshRenderer, position, rotation, scale)
//{
//	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
//	MeshRendererManager::AddMesh(mesh);
//}
//
//MeshRenderer::~MeshRenderer()
//{
//}
//
//void MeshRenderer::Update(float DeltaTime)
//{
//	GameObject::Update(DeltaTime);
//	mesh->Update(GameObjectTransform, glm::mat4(1.0f));
//}
//
//void MeshRenderer::Destroy()
//{
//	mesh->Destroy();
//}
