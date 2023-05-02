#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class MeshPicker3DPipeline : public GraphicsPipeline
{
public:
	MeshPicker3DPipeline();
	~MeshPicker3DPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

