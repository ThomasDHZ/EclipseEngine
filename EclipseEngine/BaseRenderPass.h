#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"

class BaseRenderPass
{
private:

protected:
	VkRenderPass RenderPass = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> CommandBuffer;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	glm::ivec2 RenderPassResolution;

	VkWriteDescriptorSetAccelerationStructureKHR AddAcclerationStructureBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkAccelerationStructureKHR& handle);
	VkDescriptorImageInfo AddRayTraceStorageImageDescriptor(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkImageLayout ImageLayout, VkImageView& ImageView);

	void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, uint32_t DescriptorCount, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddNullDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber);

public:

	BaseRenderPass();
	~BaseRenderPass();

	void OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer);
	virtual void SetUpCommandBuffers();
	virtual void Destroy();

	VkCommandBuffer GetCommandBuffer() { return CommandBuffer[VulkanRenderer::GetCMDIndex()]; }
};
