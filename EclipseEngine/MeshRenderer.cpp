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
	mesh->SetMeshPosition(position);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation) : ComponentRenderer(ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : ComponentRenderer(ComponentType::kMeshRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);
	mesh->SetMeshScale(scale);

	model = std::make_shared<Model>(Model(mesh));
}

MeshRenderer::MeshRenderer(const std::string& FilePath) : ComponentRenderer(ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(FilePath));
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