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
#include "JsonGraphicsPipeline.h"

class PBRReflectionSkyRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	JsonGraphicsPipeline skyboxPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRReflectionSkyRenderPass();
	~PBRReflectionSkyRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void OneTimeDraw(std::vector<std::shared_ptr<GameObject>>& gameObjectList, PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, std::shared_ptr<Mesh> reflectingMesh = nullptr);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<GameObject>>& gameObjectList, std::shared_ptr<Mesh> reflectingMesh = nullptr);
	void Destroy();
};