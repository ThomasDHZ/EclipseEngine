#include "GridRenderer2D.h"

GridRenderer2D::GridRenderer2D()
{
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec3& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec4& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
	MeshRendererManager::AddMesh(GridObject);
}

GridRenderer2D::~GridRenderer2D()
{
}

void GridRenderer2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	GridObject->Update(GameObjectTransform, glm::mat4(1.0f));
}