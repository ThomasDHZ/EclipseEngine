#pragma once
#include "VulkanRenderer.h"
#include "VulkanMenu.h"
#include "JsonConverter.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class AddPipelineMenu
{
private:
	//const char* colorWriteMask = VulkanMenu::VkColorComponentFlagBitsEnumList[0];
	bool blendEnable = 0;
	bool BlendRed = 1;
	bool BlendGreen = 1;
	bool BlendBlue = 1;
	bool BlendAlpha = 1;
	const char* srcColorBlendFactor = VulkanMenu::VkBlendFactorEnumList[0];
	const char* dstColorBlendFactor = VulkanMenu::VkBlendFactorEnumList[0];
	const char* colorBlendOp = VulkanMenu::VkBlendOpEnumList[0];
	const char* srcAlphaBlendFactor = VulkanMenu::VkBlendFactorEnumList[0];
	const char* dstAlphaBlendFactor = VulkanMenu::VkBlendFactorEnumList[0];
	const char* alphaBlendOp = VulkanMenu::VkBlendOpEnumList[0];
	std::vector<int> item_current_idx = { 0 };
public:
	AddPipelineMenu();
	~AddPipelineMenu();

	void ImGuiUpdate(std::string& ColorBlendingName, int id);
	void SaveAddColorBlendAttachmentState(nlohmann::json& json);
};

