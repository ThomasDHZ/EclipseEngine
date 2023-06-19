#pragma once
#include "VulkanRenderer.h"
#include "VulkanMenu.h"
#include "JsonConverter.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class AddTextureAttachmentMenu
{
private:
	const char* loadOpSelection = VulkanMenu::VkAttachmentLoadOpEnumList[0];
	const char* storeOpSelection = VulkanMenu::VkAttachmentStoreOpEnumList[0];
	const char* stencilLoadOpSelection = VulkanMenu::VkAttachmentLoadOpEnumList[0];
	const char* stencilStoreOpSelection = VulkanMenu::VkAttachmentStoreOpEnumList[0];
	const char* initialLayoutSelection = VulkanMenu::VkImageLayoutEnumList[0];
	const char* finalLayoutSelection = VulkanMenu::VkImageLayoutEnumList[0];

public:
    AddTextureAttachmentMenu();
	~AddTextureAttachmentMenu();

	void ImGuiUpdate(std::string& textureName, int id);
	void SaveTextureAttachmentDescription(nlohmann::json& json);
};

