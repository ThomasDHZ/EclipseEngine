#pragma once
#include <GLTF_GraphicsPipeline.h>
#include "GLTF_Temp_Model.h"
#include <GraphicsPipelineLoader.h>

class GLTFPBRRenderPIpeline : public GLTF_GraphicsPipeline
{
public:
	GLTFPBRRenderPIpeline();
	~GLTFPBRRenderPIpeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<GLTF_Temp_Model> modelList);
	void Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model model, uint32_t descriptorsetIndex);
};

