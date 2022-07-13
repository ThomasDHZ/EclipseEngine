#include "GridRendererComponent.h"
#include "LineMesh2D.h"
#include "MeshRendererManager.h"

GridRendererComponent::GridRendererComponent()
{
}

GridRendererComponent::GridRendererComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID) : Component(parentGameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Grid = std::make_shared<LineMesh2D>(LineMesh2D(GridSizeX, GridSizeY, GridSpacingX, GridSpacingY, parentGameObjectID));
	MeshRendererManager::AddMesh(Grid);
}

GridRendererComponent::~GridRendererComponent()
{
}

void GridRendererComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Grid->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void GridRendererComponent::Destroy()
{
	Grid->Destroy();
}
