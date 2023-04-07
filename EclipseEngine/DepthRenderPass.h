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
	std::shared_ptr<RenderedDepthTexture> RenderPassDepthTexture;
	DepthPipeline depthPipeline;
	DepthInstancedPipeline depthInstancedPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();
	void ClearTextureList();

public:
	DepthRenderPass();
	~DepthRenderPass();

	std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;

	void BuildRenderPass(std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList, glm::vec2 TextureResolution);
	VkCommandBuffer Draw();
	void OneTimeDraw(std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList, glm::vec2 TextureResolution);
	void Destroy();
};