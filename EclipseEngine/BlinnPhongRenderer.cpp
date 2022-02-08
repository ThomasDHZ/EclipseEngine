#include "BlinnPhongRenderer.h"

BlinnPhongRenderer::BlinnPhongRenderer()
{
}

BlinnPhongRenderer::~BlinnPhongRenderer()
{
}

void BlinnPhongRenderer::StartUp()
{
	blinnPhongRenderPass.StartUp();
	frameBufferRenderPass.StartUp(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Update()
{
}

void BlinnPhongRenderer::RebuildRenderers()
{
	blinnPhongRenderPass.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(blinnPhongRenderPass.RenderedTexture);
}

void BlinnPhongRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	blinnPhongRenderPass.Draw(sceneProperites);
	CommandBufferSubmitList.emplace_back(blinnPhongRenderPass.GetCommandBuffer());

	frameBufferRenderPass.Draw();
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.GetCommandBuffer());
}

void BlinnPhongRenderer::Destroy()
{
	blinnPhongRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
