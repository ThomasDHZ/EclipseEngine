#pragma once
#include "BaseRenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "BlinnPhongPipeline.h"
#include "DrawLinePipeline.h"
#include "WireframePipeline.h"

class BlinnPhongRenderPass : public BaseRenderPass
{
private:
	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	BlinnPhongRenderPass();
	~BlinnPhongRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	std::shared_ptr<BlinnPhongPipeline> blinnphongPipeline;
	std::shared_ptr<DrawLinePipeline> drawLinePipeline;
	std::shared_ptr<WireframePipeline> wireframePipeline;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties sceneProperties);
	void Destroy();
};

