//#include "SpriteRenderer.h"
//
//SpriteRenderer::SpriteRenderer() 
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name) : GameObject2D(Name, kSpriteRenderer)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material) : GameObject2D(Name, kSpriteRenderer)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(material, MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(material, MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(material, MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::SpriteRenderer(const std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth) : GameObject2D(Name, kSpriteRenderer, position, rotation, scale, Depth)
//{
//	Sprite = std::make_shared<Mesh2D>(Mesh2D(material, MeshSubTypeEnum::kNormal, GameObjectID));
//	MeshRendererManager::AddMesh(Sprite);
//}
//
//SpriteRenderer::~SpriteRenderer()
//{
//}
//
//void SpriteRenderer::Update(float DeltaTime)
//{
//	GameObject::Update(DeltaTime);
//	Sprite->Update(GameObjectTransform, glm::mat4(1.0f));
//}
//
//void SpriteRenderer::Destroy()
//{
//	Sprite->Destroy();
//}
//
//void SpriteRenderer::SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial)
//{
//	Sprite->SetMaterial(SpriteMaterial);
//}
