#include "ComponentRenderer.h"

ComponentRenderer::ComponentRenderer() : Component(ComponentType::kNullComponent)
{
}

ComponentRenderer::ComponentRenderer(uint64_t GameObjectID, ComponentType componentType) : Component(GameObjectID, componentType, ComponentSubType::kRenderedObject)
{
}

void ComponentRenderer::Update(float DeltaTime)
{
}

void ComponentRenderer::UpdateMeshProperties()
{
}

void ComponentRenderer::Destroy()
{
}
