#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() 
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;
	
	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name) : GameObject2D(Name)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, glm::vec2 position, uint32_t Depth) : GameObject2D(Name, position, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t Depth) : GameObject2D(Name, position, rotation, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t Depth) : GameObject2D(Name, position, rotation, scale, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material) : GameObject2D(Name)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;
	spriteLoader.materialPtr = material; 

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, glm::vec2 position, uint32_t Depth) : GameObject2D(Name, position, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;
	spriteLoader.materialPtr = material;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, glm::vec2 position, glm::vec2 rotation, uint32_t Depth) : GameObject2D(Name, position, rotation, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;
	spriteLoader.materialPtr = material;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}

SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t Depth) : GameObject2D(Name, position, rotation, scale, Depth)
{
	SpriteLoader spriteLoader = {};
	spriteLoader.GameObjectID = GameObjectID;
	spriteLoader.materialPtr = material;

	Sprite = std::make_shared<Mesh2D>(Mesh2D(spriteLoader));
	GameObjectManager::AddGameObject(std::make_shared<SpriteRenderer>(*this));
}



SpriteRenderer::SpriteRenderer(nlohmann::json& json)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	Sprite->Update(GameObjectTransform, glm::mat4(1.0f));
}

void SpriteRenderer::Destroy()
{
	Sprite->Destroy();
}

void SpriteRenderer::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	Sprite->SetMaterial(SpriteMaterial);
}
