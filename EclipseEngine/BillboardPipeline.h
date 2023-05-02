#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "GLTFSceneManager.h"

class BillboardPipeline : public GraphicsPipeline
{
public:
	BillboardPipeline();
	~BillboardPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
//	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

