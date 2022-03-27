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
	model->Update();
}

void ComponentRenderer::Destroy()
{
}

//void ComponentRenderer::SetBufferIndex(int bufferIndex)
//{
//	mesh->SetBufferIndex(bufferIndex);
//}
//
//void ComponentRenderer::SetMaterial(std::shared_ptr<Material> material)
//{
//	mesh->SetMaterial(material);
//}
