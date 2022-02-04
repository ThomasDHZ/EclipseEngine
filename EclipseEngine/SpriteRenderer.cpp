#include "SpriteRenderer.h"
#include "Vertex.h"

SpriteRenderer::SpriteRenderer() : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	mesh = Mesh(vertices, indices);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
}

void SpriteRenderer::Destroy()
{
	mesh.Destory();
}
