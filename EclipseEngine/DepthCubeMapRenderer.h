#pragma once
#include "RenderPass.h"
#include "DepthCubeMapPipeline.h"
#include "RenderedCubeMapDepthTexture.h"

class DepthCubeMapRenderer : public RenderPass
{
private:
	CubeMapSamplerBuffer cubeMapSampler;

	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	DepthCubeMapPipeline depthCubeMapPipeline;
	


	void RenderPassDesc();
	void BuildRenderPassPipelines();
	void ClearTextureList();

public:
	DepthCubeMapRenderer();
	~DepthCubeMapRenderer();

	std::shared_ptr<RenderedCubeMapTexture> DrawToCubeMap;
	std::shared_ptr<RenderedCubeMapDepthTexture> RenderPassDepthTexture;
	std::shared_ptr<RenderedDepthTexture> CubeMapSide[6];

	float Near = 0.1f;
	float Far = 10000.0f;
	glm::vec3 Position = glm::vec3(-0.0114120245f, 0.538504183f, 0.0162323173f);

	void BuildRenderPass(std::vector<std::shared_ptr<GLTFPointLight>> PointLightList, glm::vec2 TextureResolution);
	void OneTimeDraw(std::vector<std::shared_ptr<GLTFPointLight>> PointLightList, glm::vec2 TextureResolution, int lightIndex);

	VkCommandBuffer Draw(int lightIndex);
	void Destroy();
};