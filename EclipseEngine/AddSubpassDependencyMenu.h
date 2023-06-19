#pragma once
#include "VulkanRenderer.h"
#include "VulkanMenu.h"
#include "JsonConverter.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class AddSubpassDependencyMenu
{
private:

	int srcSubpass = 0;
	int dstSubpass = 0;
	const char* srcStageMask = VulkanMenu::VkPipelineStageFlagBitsEnumList[0];
	const char* dstStageMask = VulkanMenu::VkPipelineStageFlagBitsEnumList[0];
	const char* srcAccessMask = VulkanMenu::VkAccessFlagsEnumList[0];
	const char* dstAccessMask = VulkanMenu::VkAccessFlagsEnumList[0];
	const char* dependencyFlags = VulkanMenu::VkDependencyFlagsEnumList[0];

public:
	AddSubpassDependencyMenu();
	~AddSubpassDependencyMenu();

	void ImGuiUpdate(std::string& dependencyName, int id);
	void SaveSubpassDependency(nlohmann::json& json);
};

