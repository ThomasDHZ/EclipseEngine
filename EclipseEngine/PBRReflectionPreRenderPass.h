#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "SkyboxMesh.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "PBRPipeline.h"
#include "SkyboxPipeline.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "OutLinePipeline.h"
#include "PBRReflectionPipeline.h"
#include "CubeMapSamplerPipeline.h"
#include "PBRInstancedReflectionPipeline.h"
#include "PBRBakeReflectionPipeline.h"
#include "TextureBaker.h"

class PBRReflectionPreRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	PBRBakeReflectionPipeline pbrPipeline;
	CubeMapSamplerPipeline skyboxPipeline;

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);
	void ClearTextureList();

public:
	PBRReflectionPreRenderPass();
	~PBRReflectionPreRenderPass();

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> ReflectionCubeMapList;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void PreRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void BakeReflectionMaps(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, uint32_t bakedTextureAtlusSize);
	VkCommandBuffer Draw();
	void Destroy();
};