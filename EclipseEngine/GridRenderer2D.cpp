#include "GridRenderer2D.h"

GridRenderer2D::GridRenderer2D()
{
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, GameObjectID));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, const glm::vec3& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, const glm::vec4& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSize, GridSize, GridSpacing, GridSpacing, Color, GameObjectID));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, GameObjectID));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec3& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
}

GridRenderer2D::GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec4& Color) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, GameObjectID));
}

GridRenderer2D::~GridRenderer2D()
{
}

void GridRenderer2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	GridObject->Update(GameObjectTransform, glm::mat4(1.0f));
}