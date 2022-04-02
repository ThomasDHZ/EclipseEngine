#include "LineRenderer.h"

LineRenderer::LineRenderer() : ComponentRenderer()
{
}

LineRenderer::LineRenderer(std::vector<LineVertex> VertexList) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VertexList));
	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VertexList));
	mesh->SetMeshPosition(position);

	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VertexList));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);

	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VertexList));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);
	mesh->SetMeshScale(scale);

	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
	mesh->SetMeshPosition(position);

	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);

	model = std::make_shared<Model>(Model(mesh));
}

LineRenderer::LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : ComponentRenderer(ComponentType::kLineRenderer)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
	mesh->SetMeshPosition(position);
	mesh->SetMeshRotation(rotation);
	mesh->SetMeshScale(scale);

	model = std::make_shared<Model>(Model(mesh));
}


LineRenderer::LineRenderer(nlohmann::json& json) : ComponentRenderer(json)
{
	//mesh = Mesh(vertices, indices);
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::Update(float DeltaTime)
{
	model->Update();
}

void LineRenderer::Destroy()
{
	model->Destroy();
}