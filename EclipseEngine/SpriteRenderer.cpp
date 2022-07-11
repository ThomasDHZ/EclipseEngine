#include "SpriteRenderer.h"
#include "Vertex.h"
#include "Mesh2D.h"

SpriteRenderer::SpriteRenderer(uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	Sprite = std::make_shared<Mesh2D>(Mesh2D(ParentGameObjectID));
	MeshRendererManager::AddMesh(Sprite);
}


SpriteRenderer::SpriteRenderer(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Sprite->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void SpriteRenderer::Destroy()
{
	Sprite->Destroy();
}

void SpriteRenderer::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	Sprite->SetMaterial(SpriteMaterial);
}
