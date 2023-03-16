#pragma once
#include <GLTF_GraphicsPipeline.h>
#include "GLTF_Temp_Model.h"
#include "ModelRenderer.h"

class GLTFPBRRenderPIpeline : public GLTF_GraphicsPipeline
{
public:
	GLTFPBRRenderPIpeline();
	~GLTFPBRRenderPIpeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<ModelRenderer> model);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<ModelRenderer> model);
};

