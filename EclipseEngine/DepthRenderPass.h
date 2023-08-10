#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "DepthPipeline.h"
#include "DepthInstancedPipeline.h"



class DepthRenderPass : public RenderPass
{
private:
	DepthPipeline DepthPipeline;
	//DepthInstancedPipeline depthInstancedPipeline;
	bool FirstDraw = true;

	void RenderPassDesc();
	void BuildRenderPassPipelines();
	void ClearTextureList();

public:
	DepthRenderPass();
	~DepthRenderPass();
	std::shared_ptr<RenderedDepthTexture> RenderPassDepthTexture;
	std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;

	void BuildRenderPass(glm::vec2 TextureResolution);
	VkCommandBuffer Draw();
	void OneTimeDraw(glm::vec2 TextureResolution);
	void Destroy();
};