#pragma once
#include <fstream>
#include <string>
#include <json/include/nlohmann/json.hpp>

#include "VulkanMenu.h"
#include "VulkanPipelineTools.h"
#include "ConstMeshInfo.h"

class JsonGraphicsPipeline : public VulkanPipelineTools
{
public:
	VulkanMenu::PipelineRenderType PipelineType;

	VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount);
	VkDescriptorPool LoadCreateDescriptorPool(nlohmann::json& json);

	JsonGraphicsPipeline();
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, PBRRenderPassTextureSubmitList& submitList);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedDepthTexture> depthTexture);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedColorTexture> colorTexture);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, CubeMapSamplerBuffer& reflectionViewBuffer, PBRRenderPassTextureSubmitList& submitList);
	JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<Texture> colorTexture1, std::shared_ptr<Texture> colorTexture2);
	~JsonGraphicsPipeline();

	void LoadGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	void SaveGraphicsPipeline(const std::string fileName, nlohmann::json& json);
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
	void DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawMesh<T>(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	void DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, SceneProperties& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	void DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, LightViewSceneData& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	template<class T>
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawInstancedMesh<T>(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, SceneProperties& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawInstancedMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	template<class T>
	void DrawLevelLayer(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawLevelLayer<T>(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawLevelLayer(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, SceneProperties& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawLevelLayer(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	template<class T>
	void DrawSprite(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawSprite<T>(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	void DrawSprite(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, SceneProperties& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawSprite(commandBuffer, DescriptorSet, ShaderPipelineLayout, constBuffer);
	}

	void DrawLine(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		gameObject->DrawLine(commandBuffer, DescriptorSet, ShaderPipelineLayout);
	}

	void DrawTexture(VkCommandBuffer& commandBuffer, TextureCreatorProperties& textureCreatorProperties)
	{
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(TextureCreatorProperties), &textureCreatorProperties);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}

	void Destroy();
};

