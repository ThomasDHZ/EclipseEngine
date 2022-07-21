#include "GridRenderer2D.h"

GridRenderer2D::GridRenderer2D()
{
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec3& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec4& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
	GameObjectManager::AddGameObject(std::make_shared<GridRenderer2D>(*this));
}

GridRenderer2D::~GridRenderer2D()
{
}

void GridRenderer2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	LineObject->Update(GameObjectTransform, glm::mat4(1.0f));
}