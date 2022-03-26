#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer() : ComponentRenderer(ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(glm::vec3 position)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation) : ComponentRenderer(ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);
	mesh->SetRotation(rotation);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : ComponentRenderer(ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetPosition(position);
	mesh->SetRotation(rotation);
	mesh->SetScale(scale);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(nlohmann::json& json) : ComponentRenderer(json)
{
	//mesh = Mesh(vertices, indices);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(float DeltaTime)
{
}

void MeshRenderer::Destroy()
{
	model->Destroy();
}