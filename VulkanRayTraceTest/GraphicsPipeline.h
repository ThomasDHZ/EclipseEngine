#pragma once
#include "VulkanEngine.h"

enum RendererType
{
	RT_ForwardRenderer,
	RT_SceneRenderer,
	RT_TextureRenderer,
	RT_ShadowRenderer
};

struct MeshInfo
{
	uint32_t MeshID;
	uint32_t ModelID;
	uint32_t MaterialID;
};


class GraphicsPipeline
{
protected:
	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(VulkanEngine& renderer, const std::vector<char>& code);

	void CreatePipeLineLayout(VulkanEngine& renderer, VkPipelineLayoutCreateInfo PipelineLayoutInfo);
	void CreatePipeLine(VulkanEngine& renderer, VkGraphicsPipelineCreateInfo PipeLineInfo);

public:
	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;

	GraphicsPipeline();
	GraphicsPipeline(VulkanEngine& renderer);
	~GraphicsPipeline();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer);

	VkDescriptorSetLayout CreateDescriptorSetLayout(VulkanEngine& renderer, std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
};