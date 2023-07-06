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
#include "AddSubpassDependencyMenu.h"
#include "AddColorBlendAttachmentState.h"
#include "AddPipelineMenu.h"

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

	const char* TextureResolutionList[34]{
	"1 x 1",
	"2 x 2",
	"4 x 4",
	"8 x 8",
	"16 x 16",
	"32 x 32",
	"64 x 64",
	"128 x 128",
	"256 x 256",
	"512 x 512",
	"1024 x 1024",
	"4096 x 4096",
	"800 x 600",
	"1024 x 768",
	"1152 x 864",
	"1176 x 664",
	"1208 x 720",
	"1208 x 768",
	"1208 x 800",
	"1208 x 960",
	"1208 x 1024",
	"1360 x 768",
	"1400 x 1050",
	"1440 x 900",
	"1600 x 900",
	"1600 x 1024",
	"1600 x 1200",
	"1680 x 1050",
	"1920 x 1080",
	"1920 x 1200",
	"2048 x 1536",
	"2560 x 1440",
	"2560 x 1600",
	"3840 x 2160"
	};

	const int AttachmentLimit = 10;
	const int DepenencyLimit = 10;

	char RenderPassName[50];
	const char* TextureResolutionSelection = TextureResolutionList[28];
	const char* ColorTextureFormatSelection = VulkanMenu::VkFormatEnumList[37];
	const char* DepthTextureFormatSelection = VulkanMenu::VkFormatEnumList[126];
	const char* MulitSamplerEnumSelection = VulkanMenu::VkSampleCountFlagBitsEnumList[0];

	int TextureImageCount = 0;

	int MultiSampledColorTextureCount = 1;
	std::vector<AddTextureAttachmentMenu> MultiSampledTextureMenuList;

	int ColorTextureCount = 1;
	std::vector<AddTextureAttachmentMenu> ColorTextureMenuList;

	int OutputColorTextureCount = 0;
	std::vector<AddTextureAttachmentMenu> OutputColorTextureMenuList;

	AddTextureAttachmentMenu DepthtextureMenu;

	bool UseOutPutDepthTexture = false;
	AddTextureAttachmentMenu OutputDepthTextureMenu;

	int SubpassDependencyCount = 0;
	std::vector<AddSubpassDependencyMenu> SubpassDependencyMenuList;

	int AddColorBlendAttachmentStateCount = 0;
	std::vector<AddColorBlendAttachmentState> ColorBlendAttachmentMenuList;

	int AddPipelineCount = 0;
	std::vector<AddPipelineMenu> AddPipelineMenuList;
	//int 

	char ShaderName[50];
	const char* PipelineTypeSelection = PipelineTypeEnumList[0];

	int BindingDescriptorNum = 0;
	std::vector<const char*> BindingSelectionList;

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
	void SaveRenderPass();
	void SavePipeline();
};

#endif