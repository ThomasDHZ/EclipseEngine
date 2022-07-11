#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer(std::vector<MeshVertex> VertexList, std::vector<uint32_t> IndexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(VertexList, IndexList, ParentGameObjectID));
}

MeshRenderer::MeshRenderer(const std::string& FilePath, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
}

MeshRenderer::MeshRenderer(nlohmann::json json, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	Component::from_json(json);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	model->Update(GameObjectMatrix);
}

void MeshRenderer::Destroy()
{
	model->Destroy();
}