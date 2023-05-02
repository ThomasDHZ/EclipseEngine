#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class GLTF_SkyboxPipeline : public GLTF_GraphicsPipeline
{
public:
	GLTF_SkyboxPipeline();
	~GLTF_SkyboxPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap);
	void Draw(VkCommandBuffer& commandBuffer);
};

