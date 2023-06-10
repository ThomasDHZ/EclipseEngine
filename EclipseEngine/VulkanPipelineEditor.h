#ifndef VulkanPipelineEditor_H
#define VulkanPipelineEditor_H

#pragma once
#include <map>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include <string>
#include <vector>
#include "JsonGraphicsPipeline.h"
#include "VulkanMenu.h"
#include "AddTextureAttachmentMenu.h"

class VulkanPipelineEditor : public VulkanPipelineTools
{
private:
	const char* PipelineDescriptorList[14] = { "kModelTransformDescriptor",
											"kMeshPropertiesDescriptor",
											"kTextureDescriptor",
											"kMaterialDescriptor",
											"kBRDFMapDescriptor",
											"kIrradianceMapDescriptor",
											"kPrefilterMapDescriptor",
											"kCubeMapDescriptor",
											"kEnvironmentDescriptor",
											"kSunLightDescriptor",
											"kDirectionalLightDescriptor",
											"kPointLightDescriptor",
											"kSpotLightDescriptor",
											"kReflectionViewDescriptor" };

	//std::vector<std::shared_ptr<RenderedColorTexture>> MultiSampledColorTextureList;
	//std::vector<std::shared_ptr<RenderedColorTexture>> ColorTextureList;
	//std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;

	int MultiSampledColorTextureCount = 0;
	std::vector<AddTextureAttachmentMenu> MultiSampledTextureMenuList;

	int ColorTextureCount = 0;
	std::vector<AddTextureAttachmentMenu> ColorTextureMenuList;

	int DepthTextureCount = 0;
	std::vector<AddTextureAttachmentMenu> DepthtextureMenuList;

	char ShaderName[50];

	int BindingDescriptorNum = 0;
	std::vector<const char*> BindingSelectionList;

	int ColorBlendAttachment = 0;
	bool					   blendEnable;
	VulkanMenu::BlendFactorMode            srcColorBlendFactor;
	VulkanMenu::BlendFactorMode            dstColorBlendFactor;
	VulkanMenu::BlendOpMode                colorBlendOp;
	VulkanMenu::BlendFactorMode            srcAlphaBlendFactor;
	VulkanMenu::BlendFactorMode            dstAlphaBlendFactor;
	VulkanMenu::BlendOpMode                alphaBlendOp;
	//VkColorComponentFlags    colorWriteMask;

	//VkPipelineDepthStencilStateCreateInfo
	bool DepthTestEnableSelecton = true;
	bool DepthWriteEnableSelecton = true;
	bool DepthBoundsTestEnableSelecton = false;
	bool StencilTestEnableSelecton = false;
	const char* DepthCompareOpperationsSelecton = VulkanMenu::DepthCompareOpperationEnumList[1];

	//VkPipelineInputAssemblyStateCreateInfo
	const char* TopologySelection = VulkanMenu::PrimitiveTopologyEnumList[2];
	bool PrimitiveRestartEnableSelection = false;

	//VkPipelineViewportStateCreateInfo
	int ScissorCount = 0;
	int ViewportCount = 0;

	//VkPipelineRasterizationStateCreateInfo
	bool DepthClampEnable = false;
	bool DepthBiasEnable = false;
	bool RasterizerDiscardEnable = false;
	float LineWidth = 1.0f;
	const char* FrontFace = VulkanMenu::FrontFaceEnumList[0];
	const char* CullMode = VulkanMenu::CullModeEnumList[2];
	const char* PolygonMode = VulkanMenu::PolygonModeEnumList[0];



	VkPipelineShaderStageCreateInfo CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
	{
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = shaderStages;
		vertShaderStageInfo.pName = "main";

		return vertShaderStageInfo;
	}

	VkDescriptorType GetBindingType(DescriptorBindingPropertiesEnum bindingType);
public:
	VulkanPipelineEditor();
	~VulkanPipelineEditor();

	void Update();
	void LoadPipeline(std::string& pipelineFile);
	void BuildPipeline();
	void SavePipeline();
};

#endif