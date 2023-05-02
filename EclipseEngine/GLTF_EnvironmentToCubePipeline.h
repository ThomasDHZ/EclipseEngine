#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"

class GLTF_EnvironmentToCubePipeline : public GLTF_GraphicsPipeline
{
public:
	GLTF_EnvironmentToCubePipeline();
	~GLTF_EnvironmentToCubePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer);
};