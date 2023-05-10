#pragma once
#include "GLTF_GraphicsPipeline.h"
#include "Model.h"
#include "JsonGraphicsPipeline.h"

class GLTFPBRRenderPIpeline : public GLTF_GraphicsPipeline
{
public:
	GLTFPBRRenderPIpeline();
	~GLTFPBRRenderPIpeline();
	VkDescriptorSet descriptorSet;
	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<Model>> modelList);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Model> model);
};
