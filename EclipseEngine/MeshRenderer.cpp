#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer(uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(glm::vec3 position, uint64_t GameObjectID)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);
	mesh->SetMeshScale(scale);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(const std::string& FilePath, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
	model->ModelRotation = rotation;
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(FilePath, ParentGameObjectID));
	model->ModelPosition = position;
	model->ModelRotation = rotation;
	model->ModelScale = scale;
	ModelManager::AddModel(model);
}

MeshRenderer::MeshRenderer(nlohmann::json json, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kMeshRenderer)
{
	ComponentRenderer::from_json(json);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(float DeltaTime)
{
	model->Update();
}

void MeshRenderer::Destroy()
{
	model->Destroy();
}