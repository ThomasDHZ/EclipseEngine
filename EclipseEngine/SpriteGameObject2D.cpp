#include "SpriteGameObject2D.h"

SpriteGameObject2D::SpriteGameObject2D()
{
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name) : GameObject2D(Name, kSpriteRenderer)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material) : GameObject2D(Name, kSpriteRenderer)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject2D::SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
{
	LoadRenderObject<Vertex2D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject2D::~SpriteGameObject2D()
{
}

void SpriteGameObject2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void SpriteGameObject2D::Destroy()
{
}

void SpriteGameObject2D::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
}