#pragma once
#include <fstream>
#include <string>
#include <json.hpp>

#include "VulkanRenderer.h"
#include "JsonConverter.h"


class JsonGraphicsPipeline
{
private:
	VkShaderModule ReadShaderFile(const std::string& filename);
protected:


public: 
	JsonGraphicsPipeline();
	~JsonGraphicsPipeline();

	VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount);

	void SaveDescriptorPoolSize(nlohmann::json& json, VkDescriptorPoolSize descriptorPoolSize);
	void SaveDescriptorLayoutSet(nlohmann::json& json, VkDescriptorSetLayoutBinding descriptorSetLayoutBinding);
	void SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo);
	void SaveBufferDescriptorSet(nlohmann::json& json, VkWriteDescriptorSet writeDescriptorSet);

	void SavePipelineDepthStencilStateCreateInfo(nlohmann::json& json, VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo);
	void SavePipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename);
	void SavePipelineInputAssemblyStateCreateInfo(nlohmann::json& json, VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo);
	void SavePipelineViewportStateCreateInfo(nlohmann::json& json, VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo);
	void SavePipelineRasterizationStateCreateInfo(nlohmann::json& json, VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo);
	void SavePipelineMultisampleStateCreateInfo(nlohmann::json& json, VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo);
	void SavePipelineColorBlendStateCreateInfo(nlohmann::json& json, VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo);

	VkDescriptorPoolSize LoadDescriptorPoolSize(nlohmann::json& json);
	VkDescriptorSetLayoutBinding LoadDescriptorLayoutSet(nlohmann::json& json);
	VkDescriptorPool LoadCreateDescriptorPool(nlohmann::json& json);

	VkPipelineShaderStageCreateInfo LoadPipelineShaderStageCreateInfo(nlohmann::json& json);
	VkPipelineDepthStencilStateCreateInfo LoadPipelineDepthStencilStateCreateInfo(nlohmann::json& json);
	VkPipelineInputAssemblyStateCreateInfo LoadPipelineInputAssemblyStateCreateInfo(nlohmann::json& json);
	VkPipelineViewportStateCreateInfo LoadPipelineViewportStateCreateInfo(nlohmann::json& json);
	VkPipelineRasterizationStateCreateInfo LoadPipelineRasterizationStateCreateInfo(nlohmann::json& json);
	VkPipelineMultisampleStateCreateInfo LoadPipelineMultisampleStateCreateInfo(nlohmann::json& json);
	VkPipelineColorBlendStateCreateInfo LoadPipelineColorBlendStateCreateInfo(nlohmann::json& json);
	VkPipelineLayoutCreateInfo LoadPipelineLayoutCreateInfo();

	void LoadGraphicsPipeline(const std::string& filename);
	void SaveGraphicsPipeline(VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename);
};

