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
	void SaveBufferDescriptorSet(nlohmann::json& json, VkWriteDescriptorSet writeDescriptorSet);

	void SaveVKPipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename);
	void SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo);
		
	VkDescriptorSetLayoutBinding LoadDescriptorLayoutSet(nlohmann::json& json);
	VkDescriptorPoolSize LoadDescriptorPoolSize(nlohmann::json& json);

	VkPipelineShaderStageCreateInfo LoadVKPipelineShaderStageCreateInfo(nlohmann::json& json);
	VkDescriptorPool LoadCreateDescriptorPool(nlohmann::json& json);

	void LoadGraphicsPipeline(const std::string& filename);
	void SaveGraphicsPipeline(VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename);
};

