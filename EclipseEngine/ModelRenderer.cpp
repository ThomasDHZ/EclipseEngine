#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader) : GameObject3D(Name, kModelRenderer)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position) : GameObject3D(Name, kModelRenderer, position)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kModelRenderer, position, rotation)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kModelRenderer, position, rotation, scale)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader) : GameObject3D(Name, kModelRenderer)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position) : GameObject3D(Name, kModelRenderer, position)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kModelRenderer, position, rotation)
{
	modelloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(modelloader));
	modelloader.GameObjectTransform = GetGameObjectMatrix();
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kModelRenderer, position, rotation, scale)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
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