#include "SpriteRenderer.h"
#include "Vertex.h"

SpriteRenderer::SpriteRenderer(uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);
	mesh->SetMeshScale(scale);

	model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
}


SpriteRenderer::SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID) : ComponentRenderer(GameObjectID, json)
{
	//mesh = Mesh(vertices, indices);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
	model->Update();
}

void SpriteRenderer::Destroy()
{
	model->Destroy();
}
