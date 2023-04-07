#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() 
{
}

SpriteRenderer::SpriteRenderer(const std::string Name) : GameObject2D(Name, kSpriteRenderer)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material) : GameObject2D(Name, kSpriteRenderer)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void SpriteRenderer::Destroy()
{
}

void SpriteRenderer::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
}