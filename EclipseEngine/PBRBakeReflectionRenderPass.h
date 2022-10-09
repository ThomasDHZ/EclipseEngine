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
#include "CubeMapViewPipeline.h"
#include "CubeMapSamplerPipeline.h"
#include "PBRInstancedReflectionPipeline.h"

class PBRBakeReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	PBRReflectionPipeline pbrPipeline;
	PBRInstancedReflectionPipeline pbrInstancedPipeline;
	CubeMapSamplerPipeline skyboxPipeline;

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);
	void ClearTextureList();

public:
	PBRBakeReflectionRenderPass();
	~PBRBakeReflectionRenderPass();

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> ReflectionCubeMapList;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BakeReflectionMaps(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};