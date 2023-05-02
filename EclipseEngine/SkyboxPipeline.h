#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class SkyboxPipeline : public GraphicsPipeline
{
public:
	SkyboxPipeline();
	~SkyboxPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap);
	void Draw(VkCommandBuffer& commandBuffer);
};

