#include "LevelGameObject.h"

LevelGameObject::LevelGameObject()
{
}

LevelGameObject::LevelGameObject(const std::string Name) : GameObject2D(Name, kLevelRenderer)
{
}

LevelGameObject::LevelGameObject(const std::string Name, const glm::vec2& position, int drawLayer) : GameObject2D(Name, kLevelRenderer, position, drawLayer)
{
}

LevelGameObject::LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer) : GameObject2D(Name, kLevelRenderer, position, rotation, drawLayer)
{
}

LevelGameObject::LevelGameObject(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer) : GameObject2D(Name, kLevelRenderer, position, rotation, scale, drawLayer)
{
}

LevelGameObject::~LevelGameObject()
{
}

void LevelGameObject::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
}

void LevelGameObject::Destroy()
{
}

void LevelGameObject::LoadLevelGameObject2D(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::vector<std::shared_ptr<Material>> materialList)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLevel2D(levelName, tileSizeInPixels, levelBounds, materialList, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

