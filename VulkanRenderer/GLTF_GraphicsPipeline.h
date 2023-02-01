//#pragma once
//#include "VulkanRenderer.h"
//#include "VulkanBuffer.h"
//#include "GLTF_GraphicsDescriptors.h"
//
//class GLTF_GraphicsPipeline
//{
//private:
//	VkShaderModule ReadShaderFile(const std::string& filename);
//
//protected:
//	VkDescriptorSet DescriptorSet;
//	VkDescriptorSetLayout DescriptorSetLayout;
//
//	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
//	void BuildShaderPipeLine(BuildGraphicsPipelineInfo& buildPipelineInfo);
//
//public:
//	GLTF_GraphicsPipeline();
//	GLTF_GraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
//	~GLTF_GraphicsPipeline();
//
//	static std::string BaseShaderFilePath;
//
//	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
//	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
//
//	virtual void UpdateGraphicsPipeLine();
//	virtual void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
//	virtual void Destroy();
//};
//
