#include "MeshRenderer.h"
#include "Vertex.h"

MeshRenderer::MeshRenderer() : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	mesh = Mesh(vertices, indices);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(float DeltaTime)
{
}

void MeshRenderer::Destroy()
{
}