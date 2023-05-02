#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class MeshPicker2DPipeline : public GraphicsPipeline
{
public:
	MeshPicker2DPipeline();
	~MeshPicker2DPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

