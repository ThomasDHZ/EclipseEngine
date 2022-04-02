#include "ComponentRenderer.h"

ComponentRenderer::ComponentRenderer() : Component(ComponentType::kNullComponent)
{
}

ComponentRenderer::ComponentRenderer(ComponentType componentType) : Component(componentType, ComponentSubType::kRenderedObject)
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
