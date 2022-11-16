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

class PBRReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	PBRReflectionPipeline pbrPipeline;
	PBRInstancedReflectionPipeline pbrInstancedPipeline;
	CubeMapSamplerPipeline skyboxPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRReflectionRenderPass();
	~PBRReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void OneTimeDraw(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, std::shared_ptr<Mesh> reflectingMesh = nullptr);
	VkCommandBuffer Draw(std::shared_ptr<Mesh> reflectingMesh = nullptr);
	void Destroy();
};