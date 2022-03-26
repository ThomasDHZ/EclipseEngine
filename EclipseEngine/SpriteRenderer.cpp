#include "SpriteRenderer.h"
#include "Vertex.h"

SpriteRenderer::SpriteRenderer() : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	model = std::make_shared<Model>(Model(mesh));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position) : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);

	model = std::make_shared<Model>(Model(mesh));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation) : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);
	mesh->SetRotation(rotation);

	model = std::make_shared<Model>(Model(mesh));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);
	mesh->SetRotation(rotation);
	mesh->SetScale(scale);

	model = std::make_shared<Model>(Model(mesh));
}


SpriteRenderer::SpriteRenderer(nlohmann::json& json) : ComponentRenderer(json)
{
	//mesh = Mesh(vertices, indices);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
}

void SpriteRenderer::Destroy()
{
	model->Destroy();
}
