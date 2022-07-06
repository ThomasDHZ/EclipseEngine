#include "SpriteRenderer.h"
#include "Vertex.h"

SpriteRenderer::SpriteRenderer(uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	Sprite = std::make_shared<Mesh2D>(Mesh2D(GameObjectID));
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	Sprite = std::make_shared<Mesh2D>(Mesh2D(GameObjectID));
	Sprite->SetMeshPosition(position);
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	Sprite = std::make_shared<Mesh2D>(Mesh2D(GameObjectID));
	Sprite->SetMeshPosition(position);
	Sprite->SetMeshRotation(rotation);
}

SpriteRenderer::SpriteRenderer(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	Sprite = std::make_shared<Mesh2D>(Mesh2D(GameObjectID));
	Sprite->SetMeshPosition(position);
	Sprite->SetMeshRotation(rotation);
	Sprite->SetMeshScale(scale);
}


SpriteRenderer::SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
	//mesh = Mesh(vertices, indices);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
	Sprite->Update();
}

void SpriteRenderer::Destroy()
{
	Sprite->Destroy();
}

void SpriteRenderer::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	Sprite->SetMaterial(SpriteMaterial);
}
