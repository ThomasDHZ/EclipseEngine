#include "GameObject2D.h"
#include "SpriteRenderer.h"
#include "LineRenderer2D.h"
#include "GridRenderer2DComponent.h"

GameObject2D::GameObject2D() : GameObject()
{
}

GameObject2D::GameObject2D(const std::string Name) : GameObject(Name)
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale, uint32_t Depth) : GameObject(Name, glm::vec3(position.x, position.y, Depth), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, std::vector<LineVertex2D>& VertexList) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(VertexList, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(VertexList, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(VertexList, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	AddComponent(std::make_shared<LineRenderer2D>(LineRenderer2D(VertexList, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSize, float GridSpacing) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSize, GridSize, GridSpacing, GridSpacing, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSize, float GridSpacing, glm::vec3& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSize, float GridSpacing, glm::vec4& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID)));
}

GameObject2D::GameObject2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer2DComponent>(GridRenderer2DComponent(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID)));
}

GameObject2D::~GameObject2D()
{
}

void GameObject2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);

	if (auto spriteRendererComponent = GetComponentByType(ComponentType::kSpriteRenderer))
	{
		auto spriteRenderer = static_cast<SpriteRenderer*>(spriteRendererComponent.get());
		spriteRenderer->Update(GameObjectTransform, DeltaTime);
	}

	if (auto lineRenderer2DComponent = GetComponentByType(ComponentType::kLineRenderer2D))
	{
		auto lineRenderer2D = static_cast<LineRenderer2D*>(lineRenderer2DComponent.get());
		lineRenderer2D->Update(GameObjectTransform, DeltaTime);
	}
}

void GameObject2D::SetGameObjectPosition(float x, float y)
{
	GameObject::SetGameObjectPosition(x,y, GameObject::GetGameObjectPosition().z);
}

void GameObject2D::SetGameObjectPosition(glm::vec2 position)
{
	GameObject::SetGameObjectPosition(position.x, position.y, GameObject::GetGameObjectPosition().z);
}

void GameObject2D::SetGameObjectDepth(float Depth)
{
	GameObject::SetGameObjectPosition(GetGameObjectPosition().x, GameObject::GetGameObjectPosition().y, Depth);
}

void GameObject2D::SetGameObjectRotation(float x, float y)
{
	GameObject::SetGameObjectRotation(x, y, 0.0f);
}

void GameObject2D::SetGameObjectRotation(glm::vec2 rotation)
{
	GameObject::SetGameObjectRotation(rotation.x, rotation.y, 0.0f);
}

void GameObject2D::SetGameObjectScale(float x, float y)
{
	GameObject::SetGameObjectRotation(x, y, 1.0f);
}

void GameObject2D::SetGameObjectScale(glm::vec2 scale)
{
	GameObject::SetGameObjectRotation(scale.x, scale.y, 1.0f);
}

void GameObject2D::SetGameObjectMaterial(std::shared_ptr<Material> SpriteMaterial)
{
	const auto component = GetComponentByType(ComponentType::kSpriteRenderer);
	const auto spriteRenderer = static_cast<SpriteRenderer*>(component.get());
	spriteRenderer->SetSpriteMaterial(SpriteMaterial);
}
