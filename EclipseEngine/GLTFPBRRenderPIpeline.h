#pragma once
#include <GraphicsPipeline.h>
#include "GLTF_Temp_Model.h"
class GLTFPBRRenderPIpeline : public GraphicsPipeline
{
public:
	GLTFPBRRenderPIpeline();
	~GLTFPBRRenderPIpeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, GLTF_Temp_Model model);
	void Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model model);
};

