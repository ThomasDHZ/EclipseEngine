#include "MeshRenderer.h"
MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList) : GameObject3D(Name)
{ 
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(std::make_shared<Mesh3D>(Mesh3D(meshLoader)));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygon;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, MeshTypeEnum meshType) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(std::make_shared<Mesh3D>(Mesh3D(meshLoader)));
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, MeshTypeEnum meshType, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, MeshTypeEnum meshType, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, MeshTypeEnum meshType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, MeshTypeEnum meshType) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, MeshTypeEnum meshType, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, MeshTypeEnum meshType, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, MeshTypeEnum meshType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = meshType;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}


MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
}

MeshRenderer::MeshRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));
	MeshRendererManager::AddMesh(mesh);
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
