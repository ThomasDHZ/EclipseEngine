#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer(const std::string& FilePath, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
	model->ModelRotation = rotation;
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kMeshRenderer, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
	model->ModelRotation = rotation;
	model->ModelScale = scale;
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