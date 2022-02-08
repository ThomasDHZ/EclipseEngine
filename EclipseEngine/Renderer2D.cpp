#include "Renderer2D.h"

Renderer2D::Renderer2D()
{
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::StartUp()
{
	renderPass2D.StartUp();
	frameBufferRenderPass.StartUp(renderPass2D.renderedTexture);
}

void Renderer2D::Update()
{
}

void Renderer2D::RebuildRenderers()
{
	renderPass2D.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
}

void Renderer2D::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	renderPass2D.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void Renderer2D::Destroy()
{
	renderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
