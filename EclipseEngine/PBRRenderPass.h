#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "PBRPipeline.h"
#include "SkyboxPipeline.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "OutLinePipeline.h"
#include "PBRInstancePipeline.h"
#include "LightDebugPipeline.h"


class PBRRenderPass : public RenderPass
{
private:
	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	PBRPipeline pbrPipeline;
	PBRInstancePipeline pbrInstancePipeline;
	LightDebugPipeline lightDebugPipeline;
	SkyboxPipeline skyboxPipeline;
	LinePipeline linePipeline;
	OutLinePipeline outLinePipeline;
	WireFramePipeline wireframePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};