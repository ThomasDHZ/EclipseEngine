#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "GLTF_GraphicsDescriptors.h"

struct PipelineInfoStruct
{
	VkRenderPass renderPass = VK_NULL_HANDLE;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachments;
	VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;
};

struct BuildVertexDescription
{
	std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions;
	VkPrimitiveTopology VertexTopology;
	VkPolygonMode PolygonMode;
	VkCullModeFlags CullMode;
};

struct BuildRenderPassDescription
{
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachments;
	VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
	uint32_t ConstBufferSize = 0;
};

struct BuildGraphicsPipelineInfo
{
	BuildVertexDescription VertexDescription;
	BuildRenderPassDescription RenderPassDescription;
};

class GLTF_GraphicsPipeline
{
private:
	VkShaderModule ReadShaderFile(const std::string& filename);

protected:
	VkDescriptorSet DescriptorSet;
	VkDescriptorSetLayout DescriptorSetLayout;

	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
	void BuildShaderPipeLine(BuildGraphicsPipelineInfo& buildPipelineInfo);

public:
	GLTF_GraphicsPipeline();
	GLTF_GraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~GLTF_GraphicsPipeline();

	static std::string BaseShaderFilePath;

	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;

	virtual void UpdateGraphicsPipeLine();
	virtual void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	virtual void Destroy();
};

