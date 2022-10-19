#pragma once
#include "VulkanRenderer.h"
#include "VulkanBuffer.h"
#include "TextureManager.h"
#include "Vertex.h"
#include "RenderedCubeMapTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"

enum VertexDescriptorTypeEnum
{
	kVertexNone,
	kVertex2D,
	kVertex3D,
	kVertex3DInstance,
	kLine2D,
	kLine3D
};

enum PipelineRendererTypeEnum
{
	kRenderMesh,
	kRenderInstanceMesh,
	kRenderDepth,
	kRenderInstanceDepth,
	kRenderSkybox,
	kRenderDepthCubeMap,
	kRenderPBRSkyBox,
	kRenderWireFrame,
	kRenderLine,
	kRenderStencil
};

struct ConstMeshInfo
{
	alignas(4) uint32_t MeshIndex;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
};

struct PipelineInfoStruct
{
	VkRenderPass renderPass = VK_NULL_HANDLE;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachments;
	VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;
};

struct BuildGraphicsPipelineInfo
{
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachments;
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VertexDescriptorTypeEnum VertexDescriptorType = kVertexNone;
	VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
	PipelineRendererTypeEnum PipelineRendererType = kRenderMesh;
	uint32_t ConstBufferSize = 0;
};

struct PBRRenderPassTextureSubmitList
{
	std::vector<std::shared_ptr<RenderedCubeMapTexture>> IrradianceTextureList;
	std::vector<std::shared_ptr<RenderedCubeMapTexture>> PrefilterTextureList;
	std::vector<std::shared_ptr<RenderedDepthTexture>> DirectionalLightTextureShadowMaps;
	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> PointLightShadowMaps;
	std::vector<std::shared_ptr<RenderedDepthTexture>> SpotLightTextureShadowMaps;
};

class GraphicsPipeline
{
private:

	VkShaderModule ReadShaderFile(const std::string& filename);

	void BuildDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void BuildShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);

	VkWriteDescriptorSet AddAccelerationBuffer(uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);

protected:
	std::vector<VkDescriptorPoolSize>  DescriptorPoolList{};
	std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo{};
	std::vector<VkWriteDescriptorSet> DescriptorList{};



public:

	GraphicsPipeline();
	GraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~GraphicsPipeline();

	static std::string BaseShaderFilePath;

	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;
	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
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
	VkDescriptorImageInfo AddTextureDescriptor(std::shared_ptr<Texture> texture);
	VkDescriptorImageInfo AddTextureDescriptor(VkImageView view, VkSampler sampler);

	void CreateGraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SubmitDescriptorSet(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList2);

	void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddNullDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber);

	virtual void UpdateGraphicsPipeLine();
	virtual void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
    virtual void Destroy();

	VkPipelineLayout GetShaderPipelineLayout() { return ShaderPipelineLayout; }
	VkPipeline GetShaderPipeline() { return ShaderPipeline; }
	VkDescriptorSet GetDescriptorSet() { return DescriptorSet; }
	VkPipelineLayout* GetShaderPipelineLayoutPtr() { return &ShaderPipelineLayout; }
	VkPipeline* GetShaderPipelinePtr() { return &ShaderPipeline; }
	VkDescriptorSet* GetDescriptorSetPtr() { return &DescriptorSet; }
};

