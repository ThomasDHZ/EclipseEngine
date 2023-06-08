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
#include "VKEnumConverters.h"

class VulkanPipelineEditor : public VulkanPipelineTools
{
private:
	struct DescriptorBindingProperties
	{
		static const char* SelectionToString(DescriptorBindingPropertiesEnum input)
		{
			switch (input)
			{
			case kModelTransformDescriptor: return "kModelTransformDescriptor"; break;
			case kMeshPropertiesDescriptor: return "kMeshPropertiesDescriptor"; break;
			case kTextureDescriptor: return "kTextureDescriptor"; break;
			case kMaterialDescriptor: return "kMaterialDescriptor"; break;
			case kBRDFMapDescriptor: return "kBRDFMapDescriptor"; break;
			case kIrradianceMapDescriptor: return "kIrradianceMapDescriptor"; break;
			case kPrefilterMapDescriptor: return "kPrefilterMapDescriptor"; break;
			case kCubeMapDescriptor: return "kCubeMapDescriptor"; break;
			case kSunLightDescriptor: return "kSunLightDescriptor"; break;
			case kDirectionalLightDescriptor: return "kDirectionalLightDescriptor"; break;
			case kPointLightDescriptor: return "kPointLightDescriptor"; break;
			case kSpotLightDescriptor: return "kSpotLightDescriptor"; break;
			case kReflectionViewDescriptor: return "kReflectionViewDescriptor"; break;
			default: return "";
			}
		}

		static DescriptorBindingPropertiesEnum SelectionToEnum(std::string input)
		{
			if (input == "kModelTransformDescriptor") return kModelTransformDescriptor;
			if (input == "kMeshPropertiesDescriptor") return kMeshPropertiesDescriptor;
			if (input == "kTextureDescriptor") return kTextureDescriptor;
			if (input == "kMaterialDescriptor") return kMaterialDescriptor;
			if (input == "kBRDFMapDescriptor") return kBRDFMapDescriptor;
			if (input == "kIrradianceMapDescriptor") return kIrradianceMapDescriptor;
			if (input == "kPrefilterMapDescriptor") return kPrefilterMapDescriptor;
			if (input == "kCubeMapDescriptor") return kCubeMapDescriptor;
			if (input == "kSunLightDescriptor") return kSunLightDescriptor;
			if (input == "kDirectionalLightDescriptor") return kDirectionalLightDescriptor;
			if (input == "kPointLightDescriptor") return kPointLightDescriptor;
			if (input == "kSpotLightDescriptor") return kSpotLightDescriptor;
			if (input == "kReflectionViewDescriptor") return kReflectionViewDescriptor;
		}
	};

	const char* PrimitiveTopologyEnumList[10] = { "VK_PRIMITIVE_TOPOLOGY_POINT_LIST",
												  "VK_PRIMITIVE_TOPOLOGY_LINE_LIST"
												  "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN",
												  "VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY",
												  "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY",
												"VK_PRIMITIVE_TOPOLOGY_MAX_ENUM" };

	struct PrimitiveTopology
	{
		static const char* SelectionToString(VkPrimitiveTopology input)
		{
			switch (input)
			{
			case VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return "VK_PRIMITIVE_TOPOLOGY_POINT_LIST"; break;
			case VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return "VK_PRIMITIVE_TOPOLOGY_LINE_LIST"; break;
			case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP"; break;
			case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST"; break;
			case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP"; break;
			case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN"; break;
			case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY: return "VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY"; break;
			case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY: return "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY"; break;
			case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY: return "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY"; break;
			case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY"; break;
			case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST: return "VK_PRIMITIVE_TOPOLOGY_PATCH_LIST"; break;
			case VK_PRIMITIVE_TOPOLOGY_MAX_ENUM: return "VK_PRIMITIVE_TOPOLOGY_MAX_ENUM"; break;
			default: return "";
			}
		}

		static VkPrimitiveTopology SelectionToEnum(std::string input)
		{
			if (input == "VK_PRIMITIVE_TOPOLOGY_POINT_LIST") return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			if (input == "VK_PRIMITIVE_TOPOLOGY_LINE_LIST") return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			if (input == "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP") return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			if (input == "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST") return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			if (input == "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP") return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			if (input == "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN") return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			if (input == "VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY") return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
			if (input == "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY") return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
			if (input == "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY") return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
			if (input == "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY") return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
			if (input == "VK_PRIMITIVE_TOPOLOGY_PATCH_LIST") return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			if (input == "VK_PRIMITIVE_TOPOLOGY_MAX_ENUM") return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}
	};

	const char* DepthCompareOpperationEnumList[9] = { "VK_COMPARE_OP_NEVER",
													"VK_COMPARE_OP_LESS",
													"VK_COMPARE_OP_EQUAL",
													"VK_COMPARE_OP_LESS_OR_EQUAL",
													"VK_COMPARE_OP_GREATER",
													"VK_COMPARE_OP_NOT_EQUAL",
													"VK_COMPARE_OP_GREATER_OR_EQUAL",
													"VK_COMPARE_OP_ALWAYS",
													"VK_COMPARE_OP_MAX_ENUM" };

	struct CompareOpperation
	{
		static const char* SelectionToString(VkCompareOp input)
		{
			switch (input)
			{
			case VK_COMPARE_OP_NEVER: return "VK_COMPARE_OP_NEVER"; break;
			case VK_COMPARE_OP_LESS: return "VK_COMPARE_OP_LESS"; break;
			case VK_COMPARE_OP_EQUAL: return "VK_COMPARE_OP_EQUAL"; break;
			case VK_COMPARE_OP_LESS_OR_EQUAL: return "VK_COMPARE_OP_LESS_OR_EQUAL"; break;
			case VK_COMPARE_OP_GREATER: return "VK_COMPARE_OP_GREATER"; break;
			case VK_COMPARE_OP_NOT_EQUAL: return "VK_COMPARE_OP_NOT_EQUAL"; break;
			case VK_COMPARE_OP_GREATER_OR_EQUAL: return "VK_COMPARE_OP_GREATER_OR_EQUAL"; break;
			case VK_COMPARE_OP_ALWAYS: return "VK_COMPARE_OP_ALWAYS"; break;
			case VK_COMPARE_OP_MAX_ENUM: return "VK_COMPARE_OP_MAX_ENUM"; break;
			default: return "";
			}
		}

		static VkCompareOp SelectionToEnum(std::string input)
		{
			if (input == "VK_COMPARE_OP_NEVER") return VK_COMPARE_OP_NEVER;
			if (input == "VK_COMPARE_OP_LESS") return VK_COMPARE_OP_LESS;
			if (input == "VK_COMPARE_OP_EQUAL") return VK_COMPARE_OP_EQUAL;
			if (input == "VK_COMPARE_OP_LESS_OR_EQUAL") return VK_COMPARE_OP_LESS_OR_EQUAL;
			if (input == "VK_COMPARE_OP_GREATER") return VK_COMPARE_OP_GREATER;
			if (input == "VK_COMPARE_OP_NOT_EQUAL") return VK_COMPARE_OP_NOT_EQUAL;
			if (input == "VK_COMPARE_OP_GREATER_OR_EQUAL") return VK_COMPARE_OP_GREATER_OR_EQUAL;
			if (input == "VK_COMPARE_OP_ALWAYS") return VK_COMPARE_OP_ALWAYS;
			if (input == "VK_COMPARE_OP_MAX_ENUM") return VK_COMPARE_OP_MAX_ENUM;
		}
	};

	const char* FrontFaceEnumList[2] = { "VK_FRONT_FACE_COUNTER_CLOCKWISE",
										 "VK_FRONT_FACE_CLOCKWISE" };

	struct FrontFace
	{
		static const char* SelectionToString(VkFrontFace input)
		{
			switch (input)
			{
			case VK_FRONT_FACE_COUNTER_CLOCKWISE: return "VK_FRONT_FACE_COUNTER_CLOCKWISE"; break;
			case VK_FRONT_FACE_CLOCKWISE: return "VK_FRONT_FACE_CLOCKWISE"; break;
			case VK_FRONT_FACE_MAX_ENUM: return "VK_FRONT_FACE_MAX_ENUM"; break;
			}
		}

		static VkFrontFace SelectionToEnum(std::string input)
		{
			if (input == "VK_FRONT_FACE_COUNTER_CLOCKWISE") return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			if (input == "VK_FRONT_FACE_CLOCKWISE") return VK_FRONT_FACE_CLOCKWISE;
			if (input == "VK_FRONT_FACE_MAX_ENUM") return VK_FRONT_FACE_MAX_ENUM;
		}
	};

	const char* CullModeEnumList[5] = { "VK_CULL_MODE_NONE",
										"VK_CULL_MODE_FRONT_BIT",
										"VK_CULL_MODE_BACK_BIT",
										"VK_CULL_MODE_FRONT_AND_BACK",
										"VK_CULL_MODE_FLAG_BITS_MAX_ENUM" };
	struct CullMode
	{
		static const char* SelectionToString(VkCullModeFlagBits input)
		{
			switch (input)
			{
			case VK_CULL_MODE_NONE: return "VK_CULL_MODE_NONE"; break;
			case VK_CULL_MODE_FRONT_BIT: return "VK_CULL_MODE_FRONT_BIT"; break;
			case VK_CULL_MODE_BACK_BIT: return "VK_CULL_MODE_BACK_BIT"; break;
			case VK_CULL_MODE_FRONT_AND_BACK: return "VK_CULL_MODE_FRONT_AND_BACK"; break;
			case VK_CULL_MODE_FLAG_BITS_MAX_ENUM: return "VK_CULL_MODE_FLAG_BITS_MAX_ENUM"; break;
			}
		}

		static VkCullModeFlagBits SelectionToEnum(std::string input)
		{
			if (input == "VK_CULL_MODE_NONE") return VK_CULL_MODE_NONE;
			if (input == "VK_CULL_MODE_FRONT_BIT") return VK_CULL_MODE_FRONT_BIT;
			if (input == "VK_CULL_MODE_BACK_BIT") return VK_CULL_MODE_BACK_BIT;
			if (input == "VK_CULL_MODE_FRONT_AND_BACK") return VK_CULL_MODE_FRONT_AND_BACK;
			if (input == "VK_CULL_MODE_FLAG_BITS_MAX_ENUM") return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
		}
	};

	const char* PolygonModeEnumList[5] = { "VK_POLYGON_MODE_FILL",
											"VK_POLYGON_MODE_LINE",
											"VK_POLYGON_MODE_POINT",
											"VK_POLYGON_MODE_FILL_RECTANGLE_NV",
											"VK_POLYGON_MODE_MAX_ENUM" };

	struct PolygonMode
	{
		static const char* SelectionToString(VkPolygonMode input)
		{
			switch (input)
			{
			case VK_POLYGON_MODE_FILL: return "VK_POLYGON_MODE_FILL"; break;
			case VK_POLYGON_MODE_LINE: return "VK_POLYGON_MODE_LINE"; break;
			case VK_POLYGON_MODE_POINT: return "VK_POLYGON_MODE_POINT"; break;
			case VK_POLYGON_MODE_FILL_RECTANGLE_NV: return "VK_POLYGON_MODE_FILL_RECTANGLE_NV"; break;
			case VK_POLYGON_MODE_MAX_ENUM: return "VK_POLYGON_MODE_MAX_ENUM"; break;
			}
		}

		static VkPolygonMode SelectionToEnum(std::string input)
		{
			if (input == "VK_POLYGON_MODE_FILL") return VK_POLYGON_MODE_FILL;
			if (input == "VK_POLYGON_MODE_LINE") return VK_POLYGON_MODE_LINE;
			if (input == "VK_POLYGON_MODE_POINT") return VK_POLYGON_MODE_POINT;
			if (input == "VK_POLYGON_MODE_FILL_RECTANGLE_NV") return VK_POLYGON_MODE_FILL_RECTANGLE_NV;
			if (input == "VK_POLYGON_MODE_MAX_ENUM") return VK_POLYGON_MODE_MAX_ENUM;
		}
	};


	const char* PipelineDescriptorList[14] = {  "kModelTransformDescriptor", 
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
												"kReflectionViewDescriptor"};

	char ShaderName[50];

	int BindingDescriptorNum = 0;
	std::vector<const char*> BindingSelectionList;

	//VkPipelineDepthStencilStateCreateInfo
	bool DepthTestEnableSelecton = true;
	bool DepthWriteEnableSelecton = true;
	bool DepthBoundsTestEnableSelecton = false;
	bool StencilTestEnableSelecton = false;
	const char* DepthCompareOpperationsSelecton = DepthCompareOpperationEnumList[1];

	//VkPipelineInputAssemblyStateCreateInfo
	const char* TopologySelection = PrimitiveTopologyEnumList[2];
	bool PrimitiveRestartEnableSelection = false;

	//VkPipelineViewportStateCreateInfo
	int ScissorCount = 0;
	int ViewportCount = 0;

	//VkPipelineRasterizationStateCreateInfo
	bool DepthClampEnable = false;
	bool DepthBiasEnable = false;
	bool RasterizerDiscardEnable = false;
	float LineWidth = 1.0f;
	const char* FrontFace = FrontFaceEnumList[0];
	const char* CullMode = CullModeEnumList[2];
	const char* PolygonMode = PolygonModeEnumList[0];



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