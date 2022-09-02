#include "MeshRenderer.h"
MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList) : GameObject3D(Name)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position) : GameObject3D(Name, position)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position) : GameObject3D(Name, position)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData) : GameObject3D(Name)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position) : GameObject3D(Name, position)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position) : GameObject3D(Name, position)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	mesh = std::make_shared<InstanceMesh3D>(InstanceMesh3D(VertexList, IndexList, instanceData, material, MeshSubTypeEnum::kNormal, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<MeshRenderer>(*this));
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	mesh->Update(GameObjectTransform, glm::mat4(1.0f));
}

void MeshRenderer::Destroy()
{
	mesh->Destroy();
}
