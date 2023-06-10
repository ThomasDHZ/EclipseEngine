#pragma once
#include "VulkanRenderer.h"
#include "VulkanEnumToMenuList.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class AddTextureAttachmentMenu
{
private:
	glm::vec2 TextureResolution = glm::vec2(0.0f);
	const char* TextureFormatSelection = VKEnumToList::VkFormatEnumList[50];
    const char* MulitSamplerEnumSelection = VKEnumToList::VkSampleCountFlagBitsEnumList[0];

public:
    AddTextureAttachmentMenu();
	~AddTextureAttachmentMenu();

	void ImGuiUpdate(std::string& textureName, int id);
    VkAttachmentDescription GetTextureAttachmentDescription();
};

