#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "SkyboxPipeline.h"
#include "OutLinePipeline.h"
#include "PBRReflectionPipeline.h"
#include "CubeMapSamplerPipeline.h"
#include "PBRInstancedReflectionPipeline.h"
#include "PBRBakeReflectionPipeline.h"
#include "TextureBaker.h"
#include "JsonGraphicsPipeline.h"

class PBRReflectionMeshRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	JsonGraphicsPipeline pbrReflectionPipeline;
	JsonGraphicsPipeline spriteReflectionPipeline;
	//PBRInstancedReflectionPipeline pbrInstancedPipeline;
	JsonGraphicsPipeline skyboxPipeline;

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;

	CubeMapSamplerBuffer ReflectionMapSampler;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);
	void ClearTextureList();
	void UpdateView(glm::vec3 reflectPoint);

public:
	PBRReflectionMeshRenderPass();
	~PBRReflectionMeshRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedReflectionCubeMap;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void PreRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};