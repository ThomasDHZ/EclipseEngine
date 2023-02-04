#pragma once
#include <GLTF_GraphicsPipeline.h>
#include "GLTF_Temp_Model.h"

class GLTF_BRDFPipeline : public GLTF_GraphicsPipeline
{
public:
	GLTF_BRDFPipeline();
	~GLTF_BRDFPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer);
};