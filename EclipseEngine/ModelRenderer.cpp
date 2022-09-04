#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh) : GameObject3D(Name)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh) : GameObject3D(Name)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath) : GameObject3D(Name)
{
	model = std::make_shared<Model>(Model(FilePath, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(FilePath, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(FilePath, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(FilePath, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<ModelRenderer>(*this));
}

ModelRenderer::ModelRenderer(nlohmann::json json) : GameObject3D(json)
{
	from_json(json);
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::LoadPrefab(nlohmann::json& json)
{
	model->LoadPrefab(json);
}

void ModelRenderer::SaveAsPrefab(nlohmann::json& json)
{
	//model->m(json);
}

void ModelRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	model->Update(GameObjectTransform);
}

void ModelRenderer::Destroy()
{
	model->Destroy();
}