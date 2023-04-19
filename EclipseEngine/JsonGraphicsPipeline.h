#pragma once
#include <fstream>
#include <string>
#include <json.hpp>

#include "VulkanRenderer.h"
#include "JsonConverter.h"
#include "GameObject.h"
#include "GLTFSceneManager.h"

enum DescriptorBindingPropertiesEnum
{
	kModelTransformDescriptor,
	kMeshPropertiesDescriptor,
	kAlbedoMapDescriptor,
	kNormalMapDescriptor,
	kMetallicRoughnessMapDescriptor,
	kAmbientOcclusionMapDescriptor,
	kAlphaMapDescriptor,
	kDepthMapDescriptor,
	kBRDFMapDescriptor,
	kIrradianceMapDescriptor,
	kPrefilterMapDescriptor,
	kCubeMapDescriptor,
	kEnvironmentDescriptor,
	kSunLightDescriptor,
	kDirectionalLightDescriptor,
	kPointLightDescriptor,
	kSpotLightDescriptor
};
class JsonGraphicsPipeline
{
private:
	static std::string BaseShaderFilePath;
	static std::string BasePipelineFilePath;

	VkShaderModule ReadShaderFile(const std::string& filename);
	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
	VkDescriptorSet CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout);

	VkWriteDescriptorSet AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);

	void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);

	
	void LoadDescriptorSets(nlohmann::json& json, std::shared_ptr<GameObject> gameObject );

protected:


public:
	VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount);

	VkDescriptorPool LoadCreateDescriptorPool(nlohmann::json& json);

	VkPipelineShaderStageCreateInfo LoadPipelineShaderStageCreateInfo(nlohmann::json& json);
	VkPipelineDepthStencilStateCreateInfo LoadPipelineDepthStencilStateCreateInfo(nlohmann::json& json);
	VkPipelineInputAssemblyStateCreateInfo LoadPipelineInputAssemblyStateCreateInfo(nlohmann::json& json);
	VkPipelineViewportStateCreateInfo LoadPipelineViewportStateCreateInfo(nlohmann::json& json);
	VkPipelineRasterizationStateCreateInfo LoadPipelineRasterizationStateCreateInfo(nlohmann::json& json);
	VkPipelineMultisampleStateCreateInfo LoadPipelineMultisampleStateCreateInfo(nlohmann::json& json, VkSampleCountFlagBits sampleCount);
	VkPipelineColorBlendStateCreateInfo LoadPipelineColorBlendStateCreateInfo(nlohmann::json& json, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments);
	VkPipelineLayoutCreateInfo LoadPipelineLayoutCreateInfo();

	void SaveDescriptorBindingLayout(nlohmann::json& json, VkDescriptorType descriptorType, DescriptorBindingPropertiesEnum descriptorBindingPropertiesEnum);
	void SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo);
	void SaveBufferDescriptorSet(nlohmann::json& json, VkWriteDescriptorSet writeDescriptorSet);

	void SavePipelineDepthStencilStateCreateInfo(nlohmann::json& json, VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo);
	void SavePipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename);
	void SavePipelineInputAssemblyStateCreateInfo(nlohmann::json& json, VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo);
	void SavePipelineViewportStateCreateInfo(nlohmann::json& json, VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo);
	void SavePipelineRasterizationStateCreateInfo(nlohmann::json& json, VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo);
	void SavePipelineMultisampleStateCreateInfo(nlohmann::json& json, VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo);
	void SavePipelineColorBlendStateCreateInfo(nlohmann::json& json, VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo);

	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet CubeMapDescriptorSet = VK_NULL_HANDLE;

	JsonGraphicsPipeline();
	JsonGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::shared_ptr<GameObject> gameObject, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	~JsonGraphicsPipeline();

	void LoadGraphicsPipeline(const char* filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::shared_ptr<GameObject> gameObject, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer);
	void SaveGraphicsPipeline(const char* fileName, nlohmann::json& json);
	void BuildAndSaveShaderPipeLine(nlohmann::json& json, BuildGraphicsPipelineInfo& buildPipelineInfo, VkDescriptorSetLayout descriptorSetLayout);
	
	template<class T>
	void DrawCubeMap(VkCommandBuffer& commandBuffer, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		GLTFSceneManager::SkyboxMesh->Draw(commandBuffer, ShaderPipelineLayout, CubeMapDescriptorSet);
	}

	void DrawCubeMap(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		GLTFSceneManager::SkyboxMesh->Draw(commandBuffer, ShaderPipelineLayout, CubeMapDescriptorSet);
	}

	template<class T>
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->Draw(commandBuffer, ShaderPipelineLayout);
	}

	template<class T>
	void DrawSprite(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawSprite(commandBuffer, ShaderPipelineLayout);
	}

	template<class T>
	void DrawLine(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> gameObject, T& constBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(T), &constBuffer);
		gameObject->DrawLine(commandBuffer, ShaderPipelineLayout, CubeMapDescriptorSet);
	}

	void DrawTexture(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &CubeMapDescriptorSet, 0, nullptr);
		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}

	void Destroy();
};

