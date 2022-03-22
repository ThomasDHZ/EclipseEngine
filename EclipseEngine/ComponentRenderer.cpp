#include "ComponentRenderer.h"

ComponentRenderer::ComponentRenderer() : Component(ComponentType::kNullComponent)
{
}

ComponentRenderer::ComponentRenderer(ComponentType componentType) : Component(componentType, ComponentSubType::kRenderedObject)
{
}

void ComponentRenderer::Draw(VkCommandBuffer& commandBuffer)
{
	model->Draw(commandBuffer);
}

void ComponentRenderer::Update(float DeltaTime)
{
}

void ComponentRenderer::UpdateMeshProperties(MeshProperties& meshProperties)
{
	model->Update(meshProperties);
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
