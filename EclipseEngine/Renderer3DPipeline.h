#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "GLTFSceneManager.h"

class Renderer3DPipeline : public GraphicsPipeline
{

public:
	Renderer3DPipeline();
	~Renderer3DPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedDepthTexture> depthTexture);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

