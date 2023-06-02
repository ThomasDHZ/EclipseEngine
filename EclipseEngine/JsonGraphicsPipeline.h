#pragma once
#include <fstream>
#include <string>
#include <json.hpp>

#include "VulkanPipelineTools.h"

class JsonGraphicsPipeline : public VulkanPipelineTools
{
private:
	static std::string BaseShaderFilePath;
	static std::string BasePipelineFilePath;

public:
	VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount);
	VkDescriptorPool LoadCreateDescriptorPool(nlohmann::json& json);

	JsonGraphicsPipeline();
	JsonGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	JsonGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, CubeMapSamplerBuffer reflectionViewBuffer);
	~JsonGraphicsPipeline();

	void LoadGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	void SaveGraphicsPipeline(const char* fileName, nlohmann::json& json);
	void BuildAndSaveShaderPipeLine(nlohmann::json& json, BuildGraphicsPipelineInfo& buildPipelineInfo, VkDescriptorSetLayout descriptorSetLayout);
	
	template<class T>
	void DrawCubeMap(VkCommandBuffer& commandBuffer, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		GLTFSceneManager::SkyboxMesh->Draw(commandBuffer, ShaderPipelineLayout, DescriptorSet);
	}

	void DrawCubeMap(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		GLTFSceneManager::SkyboxMesh->Draw(commandBuffer, ShaderPipelineLayout, DescriptorSet);
	}

	template<class T>
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->Draw(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawReflectionMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, uint32_t reflectionIndex)
	{
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
        gameObject->DrawReflectionMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout, reflectionIndex);
	}

	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawInstancedMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawSprite(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawSprite(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawLine(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawLine(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawTexture(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}

	void Destroy();
};

