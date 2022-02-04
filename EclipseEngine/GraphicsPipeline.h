#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"

struct ConstMeshInfo
{
	alignas(4) uint32_t MeshIndex;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
};

struct DescriptorSetBindingStruct
{
	uint32_t DescriptorSlotNumber;
	VkShaderStageFlags StageFlags;
	VkDescriptorType DescriptorType;
	VkWriteDescriptorSetAccelerationStructureKHR AccelerationStructureDescriptor;
	std::vector<VkDescriptorImageInfo> TextureDescriptor;
	std::vector<VkDescriptorBufferInfo> BufferDescriptor;
	uint32_t Count;
};

struct DescriptorSetLayoutBindingInfo
{
	uint32_t Binding;
	VkDescriptorType DescriptorType;
	VkShaderStageFlags StageFlags;
	uint32_t Count;
};

class GraphicsPipeline
{
private:
	std::vector<VkDescriptorPoolSize>  DescriptorPoolList{};
	std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo{};
	std::vector<VkWriteDescriptorSet> DescriptorList{};
	std::vector<DescriptorSetBindingStruct> DescriptorBindingList{};

	VkShaderModule ReadShaderFile(const std::string& filename);

	VkWriteDescriptorSet AddAccelerationBuffer(uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);

protected:
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineCache PipelineCache = VK_NULL_HANDLE;

	VkSampler NullSampler = VK_NULL_HANDLE;
	VkDescriptorImageInfo nullBufferInfo;

	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);

	VkDescriptorPoolSize AddDsecriptorPoolBinding(VkDescriptorType descriptorType, uint32_t descriptorCount);
	VkDescriptorPool CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo);
	VkDescriptorSetLayout CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
	VkDescriptorSetLayout CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo, VkDescriptorSetLayoutBindingFlagsCreateInfoEXT& DescriptorSetLayoutBindingFlags);
	VkDescriptorSet CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout);
	VkDescriptorBufferInfo AddBufferDescriptor(VulkanBuffer& buffer);

	VkWriteDescriptorSet AddAccelerationBuffer(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	VkWriteDescriptorSet AddStorageImageBuffer(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorImageInfo& TextureImageInfo);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, VkDescriptorImageInfo& TextureImageInfo);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorSet& DescriptorSet, std::vector<VkDescriptorImageInfo>& TextureImageInfo);
	VkWriteDescriptorSetAccelerationStructureKHR AddAcclerationStructureBinding(VkAccelerationStructureKHR& handle);
	VkDescriptorImageInfo AddRayTraceReturnImageDescriptor(VkImageLayout ImageLayout, VkImageView& ImageView);
	VkDescriptorImageInfo AddTextureDescriptor(VkImageView view, VkSampler sampler);

public:

	GraphicsPipeline();
	~GraphicsPipeline();

	void AddAccelerationDescriptorSetBinding(uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddNullDescriptorSetBinding(uint32_t BindingNumber);
	void SubmitDescriptorSet();

	virtual void UpdateGraphicsPipeLine();
	virtual void Destroy();

	VkPipelineLayout GetShaderPipelineLayout() { return ShaderPipelineLayout; }
	VkPipeline GetShaderPipeline() { return ShaderPipeline; }
	VkDescriptorSet GetDescriptorSet() { return DescriptorSet; }
	VkPipelineLayout* GetShaderPipelineLayoutPtr() { return &ShaderPipelineLayout; }
	VkPipeline* GetShaderPipelinePtr() { return &ShaderPipeline; }
	VkDescriptorSet* GetDescriptorSetPtr() { return &DescriptorSet; }
};

