#include "Renderer2D.h"

Renderer2D::Renderer2D()
{
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::StartUp()
{
	meshPickerRenderPass.StartUp();
	renderPass2D.StartUp();
	frameBufferRenderPass.StartUp(renderPass2D.renderedTexture);
}

void Renderer2D::Update()
{
}

void Renderer2D::RebuildRenderers()
{
	meshPickerRenderPass.RebuildSwapChain();
	renderPass2D.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
}

void Renderer2D::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	meshPickerRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(meshPickerRenderPass.GetCommandBuffer());

	renderPass2D.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(renderPass2D.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void Renderer2D::Destroy()
{
	meshPickerRenderPass.Destroy();
	renderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
