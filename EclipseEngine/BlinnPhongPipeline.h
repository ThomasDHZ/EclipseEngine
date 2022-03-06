#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class BlinnPhongPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	BlinnPhongPipeline();
	BlinnPhongPipeline(const VkRenderPass& renderPass, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	~BlinnPhongPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
};

