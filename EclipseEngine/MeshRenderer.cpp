#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer() : ComponentRenderer(ComponentType::kMeshRenderer)
{
	model = std::make_shared<Model>(Model(vertices, indices));
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