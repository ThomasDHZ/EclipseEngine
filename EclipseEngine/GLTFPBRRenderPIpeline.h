#pragma once
#include "GLTF_GraphicsPipeline.h"
#include "GLTF_Temp_Model.h"
#include "JsonGraphicsPipeline.h"

class GLTFPBRRenderPIpeline : public GLTF_GraphicsPipeline
{
public:
	GLTFPBRRenderPIpeline();
	~GLTFPBRRenderPIpeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<GLTF_Temp_Model>> modelList);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GLTF_Temp_Model> model);
};
