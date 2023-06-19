#include "AddSubpassDependencyMenu.h"

AddSubpassDependencyMenu::AddSubpassDependencyMenu()
{
}

AddSubpassDependencyMenu::~AddSubpassDependencyMenu()
{
}

void AddSubpassDependencyMenu::ImGuiUpdate(std::string& dependencyName, int id)
{
	ImGui::Separator();
	ImGui::Text(dependencyName.c_str());
	ImGui::Separator();

	ImGui::PushID(id);

	ImGui::InputInt("srcSubpass", &srcSubpass);
	ImGui::InputInt("dstSubpass", &dstSubpass);
	if (ImGui::BeginCombo("srcStageMask", srcStageMask))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkPipelineStageFlagBitsEnumList); n++)
		{
			bool is_selected = (srcStageMask == VulkanMenu::VkPipelineStageFlagBitsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkPipelineStageFlagBitsEnumList[n], is_selected))
			{
				srcStageMask = VulkanMenu::VkPipelineStageFlagBitsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("dstStageMask", dstStageMask))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkPipelineStageFlagBitsEnumList); n++)
		{
			bool is_selected = (dstStageMask == VulkanMenu::VkPipelineStageFlagBitsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkPipelineStageFlagBitsEnumList[n], is_selected))
			{
				dstStageMask = VulkanMenu::VkPipelineStageFlagBitsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("srcAccessMask", srcAccessMask))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkImageLayoutEnumList); n++)
		{
			bool is_selected = (srcAccessMask == VulkanMenu::VkAccessFlagsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAccessFlagsEnumList[n], is_selected))
			{
				srcAccessMask = VulkanMenu::VkAccessFlagsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("dstAccessMask", dstAccessMask))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkAccessFlagsEnumList); n++)
		{
			bool is_selected = (dstAccessMask == VulkanMenu::VkAccessFlagsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAccessFlagsEnumList[n], is_selected))
			{
				dstAccessMask = VulkanMenu::VkAccessFlagsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("dependencyFlags", dependencyFlags))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkDependencyFlagsEnumList); n++)
		{
			bool is_selected = (dependencyFlags == VulkanMenu::VkDependencyFlagsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkDependencyFlagsEnumList[n], is_selected))
			{
				dependencyFlags = VulkanMenu::VkDependencyFlagsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopID();

	ImGui::Separator();
}

void AddSubpassDependencyMenu::SaveSubpassDependency(nlohmann::json& json)
{
	json["srcSubpass"] = srcSubpass;
	json["dstSubpass"] = dstSubpass;
	json["srcStageMask"] = VulkanMenu::VkPipelineStageFlagBitsConverter::SelectionToEnum(srcStageMask);
	json["dstStageMask"] = VulkanMenu::VkPipelineStageFlagBitsConverter::SelectionToEnum(dstStageMask);
	json["srcAccessMask"] = VulkanMenu::VkAccessFlagsConverter::SelectionToEnum(srcAccessMask);
	json["dstAccessMask"] = VulkanMenu::VkAccessFlagsConverter::SelectionToEnum(dstAccessMask);
	json["dependencyFlags"] = VulkanMenu::VkDependencyFlagsConverter::SelectionToEnum(dependencyFlags);
}
