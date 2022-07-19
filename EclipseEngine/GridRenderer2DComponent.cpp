#include "GridRenderer2DComponent.h"
#include "LineMesh2D.h"
#include "MeshRendererManager.h"

GridRenderer2DComponent::GridRenderer2DComponent()
{
}

GridRenderer2DComponent::GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID) : Component(parentGameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Grid = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, parentGameObjectID));
	MeshRendererManager::AddMesh(Grid);
}

GridRenderer2DComponent::GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color, uint64_t parentGameObjectID) : Component(parentGameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Grid = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, parentGameObjectID));
	MeshRendererManager::AddMesh(Grid);
}

GridRenderer2DComponent::GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color, uint64_t parentGameObjectID) : Component(parentGameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Grid = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, Color, parentGameObjectID));
	MeshRendererManager::AddMesh(Grid);
}

GridRenderer2DComponent::~GridRenderer2DComponent()
{
}

void GridRenderer2DComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Grid->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void GridRenderer2DComponent::Destroy()
{
	Grid->Destroy();
}
