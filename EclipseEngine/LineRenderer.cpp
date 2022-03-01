#include "LineRenderer.h"

LineRenderer::LineRenderer() : ComponentRenderer()
{
}

LineRenderer::LineRenderer(std::vector<Vertex> VertexList) : ComponentRenderer(ComponentType::kLineRenderer)
{
	mesh = Mesh(VertexList);
}

LineRenderer::LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint) : ComponentRenderer(ComponentType::kLineRenderer)
{
	mesh = Mesh(StartPoint, EndPoint);
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
}

void LineRenderer::Destroy()
{
	mesh.Destory();
}