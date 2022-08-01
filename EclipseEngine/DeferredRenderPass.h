#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "GraphicsPipeline.h"
#include "DeferredRendererPipeline.h"

class DeferredRenderPass : public RenderPass
{
private:
	std::shared_ptr<RenderedColorTexture> ColorTexture;

	DeferredRendererPipeline DeferredPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedColorTexture> PositionTexture,
		std::shared_ptr<RenderedColorTexture> TangentTexture,
		std::shared_ptr<RenderedColorTexture> BiTangentTexture,
		std::shared_ptr<RenderedColorTexture> TBNormalTexture,
		std::shared_ptr<RenderedColorTexture> NormalTexture,
		std::shared_ptr<RenderedColorTexture> AlbedoTexture,
		std::shared_ptr<RenderedColorTexture> SpecularTexture,
		std::shared_ptr<RenderedColorTexture> BloomTexture,
		std::shared_ptr<RenderedColorTexture> ShadowTexture);

public:
	DeferredRenderPass();
	~DeferredRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::shared_ptr<RenderedColorTexture> PositionTexture,
		std::shared_ptr<RenderedColorTexture> TangentTexture,
		std::shared_ptr<RenderedColorTexture> BiTangentTexture,
		std::shared_ptr<RenderedColorTexture> TBNormalTexture,
		std::shared_ptr<RenderedColorTexture> NormalTexture,
		std::shared_ptr<RenderedColorTexture> AlbedoTexture,
		std::shared_ptr<RenderedColorTexture> SpecularTexture,
		std::shared_ptr<RenderedColorTexture> BloomTexture,
		std::shared_ptr<RenderedColorTexture> ShadowTexture);

	VkCommandBuffer Draw();
	void Destroy();
};

