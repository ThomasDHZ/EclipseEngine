#pragma once
#include "VulkanRenderer.h"

class GLTF_GraphicsDescriptors
{
private:
	static VkDescriptorPoolSize AddDescriptorPoolBinding(VkDescriptorType descriptorType, uint32_t descriptorCount);
	static VkDescriptorSetLayout CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);

	static VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo);

	static VkWriteDescriptorSet AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	static VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	static VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	static VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	static VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);

public:

	static VkWriteDescriptorSet WriteBufferDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo Buffer);
	static VkWriteDescriptorSet WriteBufferDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> Buffer);
	static VkWriteDescriptorSet WriteTextureDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo TextureBuffer);

	static VkDescriptorSet CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout);

	static VkDescriptorSet SubmitDescriptorSet(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	static VkDescriptorSetLayout SubmitDescriptorSetLayout(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);

	static VkDescriptorPool CreateDesciptorPool(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	static VkDescriptorSetLayout CreateDescriptorSetLayout(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	static VkDescriptorSet CreateDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);

	static void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	static void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
};

