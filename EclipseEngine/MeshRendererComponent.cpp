#include "MeshRendererComponent.h"
#include "Vertex.h"

MeshRendererComponent::MeshRendererComponent(std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(VertexList, IndexList, ParentGameObjectID));
}

MeshRendererComponent::MeshRendererComponent(const std::string& FilePath, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
}

MeshRendererComponent::MeshRendererComponent(nlohmann::json json, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	Component::from_json(json);
}

MeshRendererComponent::~MeshRendererComponent()
{
}

void MeshRendererComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	model->Update(GameObjectMatrix);
}

void MeshRendererComponent::Destroy()
{
	model->Destroy();
}