#include "GridRenderer3DComponent.h"

GridRenderer3DComponent::GridRenderer3DComponent()
{
}

GridRenderer3DComponent::GridRenderer3DComponent(int GridSize, float GridSpacing, uint64_t parentGameObjectID)
{
	ParentGameObjectID = parentGameObjectID;
	Grid = std::make_shared<Model>(Model(GridSize, GridSpacing, parentGameObjectID));
}

GridRenderer3DComponent::GridRenderer3DComponent(int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ, uint64_t parentGameObjectID) : Component(parentGameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	ParentGameObjectID = parentGameObjectID;
	Grid = std::make_shared<Model>(Model(GridSizeX, GridSizeY, GridSizeZ, GridSpacingX, GridSpacingY, GridSpacingZ, parentGameObjectID));
}

GridRenderer3DComponent::~GridRenderer3DComponent()
{
}

void GridRenderer3DComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Grid->Update(GameObjectMatrix);
}

void GridRenderer3DComponent::Destroy()
{
	Grid->Destroy();
}