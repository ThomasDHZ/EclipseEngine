#include "SpriteRendererComponent.h"
#include "Vertex.h"
#include "Mesh2D.h"

SpriteRendererComponent::SpriteRendererComponent(uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kSpriteRenderer, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = GameObjectID;
	Sprite = std::make_shared<Mesh2D>(Mesh2D(ParentGameObjectID));
	MeshRendererManager::AddMesh(Sprite);
}


SpriteRendererComponent::SpriteRendererComponent(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
}

SpriteRendererComponent::~SpriteRendererComponent()
{
}

void SpriteRendererComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Sprite->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void SpriteRendererComponent::Destroy()
{
	Sprite->Destroy();
}

void SpriteRendererComponent::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	Sprite->SetMaterial(SpriteMaterial);
}
