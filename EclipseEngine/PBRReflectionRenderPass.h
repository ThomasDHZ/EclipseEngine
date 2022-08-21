#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "SkyboxMesh.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "SkyboxPipeline.h"
#include "PBRReflectionPipeline.h"
#include "SkyboxPipeline.h"
#include "CubeMapSamplerPipeline.h"

class PBRReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	PBRReflectionPipeline pbrPipeline;
	CubeMapSamplerPipeline skyboxPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRReflectionRenderPass();
	~PBRReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> ReflectionCubeMapTexture;
	std::shared_ptr<RenderedCubeMapTexture> BloomTexture;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	void OneTimeDraw(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, glm::vec3 DrawPosition = glm::vec3(0.0f));
	VkCommandBuffer Draw(glm::vec3 DrawPosition);
	void Destroy();
};

