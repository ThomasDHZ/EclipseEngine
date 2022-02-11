#include "ComponentRenderer.h"

ComponentRenderer::ComponentRenderer(ComponentType componentType) : Component(componentType)
{
}

void ComponentRenderer::Draw(VkCommandBuffer& commandBuffer)
{
	mesh.Draw(commandBuffer);
}

void ComponentRenderer::Update(float DeltaTime)
{
}

void ComponentRenderer::UpdateMeshProperties(MeshProperties& meshProperties)
{
	mesh.UpdateMeshProperties(meshProperties);
}

void ComponentRenderer::Destroy()
{
}

void ComponentRenderer::SetBufferIndex(int bufferIndex)
{
	mesh.SetBufferIndex(bufferIndex);
}

void ComponentRenderer::SetMaterial(std::shared_ptr<Material> material)
{
	mesh.SetMaterial(material);
}
