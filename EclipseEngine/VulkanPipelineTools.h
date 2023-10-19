#pragma once
#include "VulkanRenderer.h"
#include "JsonConverter.h"
#include "GameObject.h"
#include "GLTFSceneManager.h"
#include "GLTF_GraphicsPipeline.h"


enum DescriptorBindingPropertiesEnum
{
	kModelTransformDescriptor,
	kMeshPropertiesDescriptor,
	kTextureDescriptor,
	kMaterialDescriptor,
	kBRDFMapDescriptor,
	kIrradianceMapDescriptor,
	kPrefilterMapDescriptor,
	kCubeMapDescriptor,
	kEnvironmentDescriptor,
	kSunLightDescriptor,
	kDirectionalLightDescriptor,
	kPointLightDescriptor,
	kSpotLightDescriptor,
	kReflectionViewDescriptor,
	kDirectionalShadowDescriptor,
	kPointShadowDescriptor,
	kSpotShadowDescriptor,
	kViewTextureDescriptor,
	kViewDepthTextureDescriptor,
	kCubeMapSamplerDescriptor,
	kRotatingPaletteTextureDescriptor,
	kMathOpperation1Descriptor,
	kMathOpperation2Descriptor,
};

struct DescriptorBindingLock
{
	bool Used = false;
	VkDescriptorImageInfo ImageInfo;
};

class VulkanPipelineTools
{
private:
protected:
	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;

	VkDescriptorBufferInfo ReflectionViewBuffer;
	DescriptorBindingLock ReflectionIrradianceBuffer;
	DescriptorBindingLock ReflectionPrefilterBuffer;
	DescriptorBindingLock DirectionalShadowBuffer;
	std::vector<VkDescriptorImageInfo> PointShadowBuffers;
	DescriptorBindingLock SpotShadowBuffer;
	DescriptorBindingLock CubeMapBuffer;
	DescriptorBindingLock TextureBuffer;
	DescriptorBindingLock DepthBuffer;
	DescriptorBindingLock MathOpperationTexture1Buffer;
	DescriptorBindingLock MathOpperationTexture2Buffer;

	VkShaderModule ReadGLSLShaderFile(const std::string& filename);
	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages);
	VkDescriptorSet CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout);
	
	void LoadDescriptorSets(nlohmann::json& json);

	void AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	void AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags = VK_SHADER_STAGE_ALL);
	
	VkWriteDescriptorSet AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType);
	VkWriteDescriptorSet AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType);

	VkPipelineDepthStencilStateCreateInfo LoadPipelineDepthStencilStateCreateInfo(nlohmann::json& json);
	VkPipelineInputAssemblyStateCreateInfo LoadPipelineInputAssemblyStateCreateInfo(nlohmann::json& json);
	VkPipelineViewportStateCreateInfo LoadPipelineViewportStateCreateInfo(nlohmann::json& json);
	VkPipelineRasterizationStateCreateInfo LoadPipelineRasterizationStateCreateInfo(nlohmann::json& json);
	VkPipelineMultisampleStateCreateInfo LoadPipelineMultisampleStateCreateInfo(nlohmann::json& json, VkSampleCountFlagBits sampleCount);
	VkPipelineColorBlendStateCreateInfo LoadPipelineColorBlendStateCreateInfo(nlohmann::json& json, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments);
	
	std::vector<VkPipelineColorBlendAttachmentState> LoadPipelineColorBlendAttachmentStateInfo(nlohmann::json& json);

	VkPipelineLayoutCreateInfo LoadPipelineLayoutCreateInfo();

	void LoadReflectionSamplerBuffer(CubeMapSamplerBuffer& reflectionMapSampler);
	void LoadReflectionIrradianceMapBuffer(std::shared_ptr<RenderedCubeMapTexture>	 reflectionIrradianceMap);
	void LoadReflectionPrefilterMapBuffer(std::shared_ptr<RenderedCubeMapTexture>	 reflectionPrefilterMap);
	void LoadDirectionalShadowBuffer(std::shared_ptr<RenderedDepthTexture>	 directionalShadow);
	void LoadPointShadowBuffers(std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> pointShadowTextures);
	void LoadSpotShadowBufferr(std::shared_ptr<RenderedDepthTexture>	 spotShadow);
	void LoadCubeMapBuffer(std::shared_ptr<RenderedCubeMapTexture>	 cubeMap);
	void LoadTextureBuffer(std::shared_ptr<RenderedColorTexture>	 colorTexture);
	void LoadDepthTextureBuffer(std::shared_ptr<RenderedDepthTexture>	 depthTexture);
	void LoadMathOpperationTexture1Buffer(std::shared_ptr<Texture>	texture);
	void LoadMathOpperationTexture2Buffer(std::shared_ptr<Texture>	texture);
public: 
	VkPipelineShaderStageCreateInfo LoadPipelineShaderStageCreateInfo(nlohmann::json& json);

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

	void SavePipelineColorBlendAttachmentStateInfo(nlohmann::json& json, std::vector<VkPipelineColorBlendAttachmentState>& pipelineColorBlendAttachmentStateList);
};

