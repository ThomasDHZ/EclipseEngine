#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "GraphicsPipeline.h"

class MeshPickerRenderPass2D : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> MeshPickerPipeline;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	MeshPickerRenderPass2D();
	~MeshPickerRenderPass2D();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties& sceneProperties);
	void Destroy();

	Pixel ReadPixel(glm::ivec2 PixelTexCoord);
};