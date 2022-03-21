#include "SpriteRenderer.h"
#include "Vertex.h"

SpriteRenderer::SpriteRenderer() : ComponentRenderer(ComponentType::kSpriteRenderer)
{
	mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
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
	mesh->Destory();
}
