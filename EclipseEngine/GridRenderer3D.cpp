#include "GridRenderer3D.h"

GridRenderer3D::GridRenderer3D()
{
}

GridRenderer3D::GridRenderer3D(const std::string Name, int GridSize, float GridSpacing) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh3D>(LineMesh3D(GridSize, GridSize, GridSize, GridSpacing, GridSpacing, GridSpacing));
}

GridRenderer3D::GridRenderer3D(const std::string Name, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ) : GameObject(Name)
{
	GridObject = std::make_shared<LineMesh3D>(LineMesh3D(GridSizeX, GridSizeY, GridSizeZ, GridSpacingX, GridSpacingY, GridSpacingZ));
}

GridRenderer3D::~GridRenderer3D()
{
}

void GridRenderer3D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	GridObject->Update(GameObjectTransform, glm::mat4(1.0f));
}
