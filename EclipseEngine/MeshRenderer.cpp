#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : Component(ComponentType::kSpriteRenderer)
{
	std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	std::vector<uint32_t> indices = {
	   0, 1, 2
	};

	mesh = Mesh(vertices, indices);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(float DeltaTime)
{
}

void MeshRenderer::Draw(VkCommandBuffer& commandBuffer)
{
	mesh.Draw(commandBuffer);
}
