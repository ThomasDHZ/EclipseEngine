#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include <string>
#include <vector>
#include "JsonGraphicsPipeline.h"

class VulkanPipelineEditor
{
private:
	const char* PipelineDescriptorList[13] = {  "kModelTransformDescriptor", 
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
												"kSpotLightDescriptor" };

	const char* PrimitiveTopologySelection[9] = { "VK_PRIMITIVE_TOPOLOGY_POINT_LIST",
												  "VK_PRIMITIVE_TOPOLOGY_LINE_LIST"
												  "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST", 
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN", 
												  "VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY",
												  "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY", 
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY",
												  "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY" };

	const char* DepthCompareOpperations[8] = {   "VK_COMPARE_OP_NEVER",
												"VK_COMPARE_OP_LESS",
												"VK_COMPARE_OP_EQUAL",
												"VK_COMPARE_OP_LESS_OR_EQUAL",
												"VK_COMPARE_OP_GREATER",
												"VK_COMPARE_OP_NOT_EQUAL",
												"VK_COMPARE_OP_GREATER_OR_EQUAL",
												"VK_COMPARE_OP_ALWAYS" };

	char VertexShaderLocation[50];
	char PixelShaderLocation[50];

	int BindingDescriptorNum = 0;
	std::vector<const char*> BindingSelectionList;

	//VkPipelineDepthStencilStateCreateInfo
	bool DepthTestEnableSelecton = true;
	bool DepthWriteEnableSelecton = true;
	bool DepthBoundsTestEnableSelecton = false;
	bool StencilTestEnableSelecton = false;
	int DepthCompareOpperationsSelecton = 1;

	//VkPipelineInputAssemblyStateCreateInfo
	int TopologySelection = 0;
	bool PrimitiveRestartEnableSelection = false;



	char SavePipelineName[50];

public:
	VulkanPipelineEditor();
	~VulkanPipelineEditor();

	void Update();
};

