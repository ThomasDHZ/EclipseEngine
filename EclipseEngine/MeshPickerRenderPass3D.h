#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "GraphicsPipeline.h"

class MeshPickerRenderPass3D : public RenderPass
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
	MeshPickerRenderPass3D();
	~MeshPickerRenderPass3D();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties& sceneProperties);
	void Destroy();

	Pixel ReadPixel(glm::ivec2 PixelTexCoord);
};