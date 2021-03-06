#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "GameObjectManager.h"
#include "GraphicsPipeline.h"
#include "TopLevelAccelerationStructureManager.h"
#include "SceneManager.h"

#include <fstream>

enum RenderPassTypeEnum
{
	kRenderPassDynamic,
	kRenderPassPreRender,
	kRenderPassBake
};

class RenderPass
{
private:
	VkShaderModule ReadShaderFile(const std::string& filename);

protected:
	RenderPassTypeEnum RenderPassType = RenderPassTypeEnum::kRenderPassDynamic;
	VkRenderPass renderPass = VK_NULL_HANDLE;

	std::vector<VkCommandBuffer> CommandBuffer;
	std::vector<VkFramebuffer> RenderPassFramebuffer;
	glm::ivec2 RenderPassResolution;

	VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;

	VkWriteDescriptorSetAccelerationStructureKHR AddAcclerationStructureBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkAccelerationStructureKHR* handle);
	VkDescriptorImageInfo AddRayTraceStorageImageDescriptor(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkImageLayout ImageLayout, VkImageView& ImageView);
	VkDescriptorImageInfo AddTextureDescriptor(std::shared_ptr<Texture> texture);
	VkDescriptorImageInfo AddTextureDescriptor(VkImageView view, VkSampler sampler);

	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
	virtual void CreateRendererFramebuffers(std::vector<VkImageView>& AttachmentList);

	void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo,VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddNullDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber);

public:

	RenderPass();
	~RenderPass();

	void OneTimeRenderPassSubmit(VkCommandBuffer* CMDBuffer);
	virtual void DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, ConstSkyBoxView& sceneProperties);
	virtual void DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, PrefilterSkyboxSettings& prefilterSkyboxSettings);
	virtual void DrawSkybox(std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh);
	virtual void SetUpCommandBuffers();
	virtual void Destroy();

	std::vector<std::shared_ptr<Mesh>> GetObjectRenderList(std::shared_ptr<GameObject> obj);
	RenderPassTypeEnum GetRenderPassType() { return RenderPassType; }
	VkCommandBuffer GetCommandBuffer() { return CommandBuffer[VulkanRenderer::GetCMDIndex()]; }
};
