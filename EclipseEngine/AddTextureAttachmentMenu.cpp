#include "AddTextureAttachmentMenu.h"

AddTextureAttachmentMenu::AddTextureAttachmentMenu()
{
}

AddTextureAttachmentMenu::~AddTextureAttachmentMenu()
{
}

void AddTextureAttachmentMenu::ImGuiUpdate(std::string& textureName, int id)
{
	ImGui::Separator();
	ImGui::Text(textureName.c_str());
	ImGui::Separator();

	ImGui::PushID(id);

	if (ImGui::BeginCombo("Load Op", loadOpSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkAttachmentLoadOpEnumList); n++)
		{
			bool is_selected = (loadOpSelection == VulkanMenu::VkAttachmentLoadOpEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAttachmentLoadOpEnumList[n], is_selected))
			{
				loadOpSelection = VulkanMenu::VkAttachmentLoadOpEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Store Op", storeOpSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkAttachmentStoreOpEnumList); n++)
		{
			bool is_selected = (storeOpSelection == VulkanMenu::VkAttachmentStoreOpEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAttachmentStoreOpEnumList[n], is_selected))
			{
				storeOpSelection = VulkanMenu::VkAttachmentStoreOpEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Stencil Load Op", stencilLoadOpSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkAttachmentLoadOpEnumList); n++)
		{
			bool is_selected = (stencilLoadOpSelection == VulkanMenu::VkAttachmentLoadOpEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAttachmentLoadOpEnumList[n], is_selected))
			{
				stencilLoadOpSelection = VulkanMenu::VkAttachmentLoadOpEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Stencil Store Op", stencilStoreOpSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkAttachmentStoreOpEnumList); n++)
		{
			bool is_selected = (stencilStoreOpSelection == VulkanMenu::VkAttachmentStoreOpEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkAttachmentStoreOpEnumList[n], is_selected))
			{
				stencilStoreOpSelection = VulkanMenu::VkAttachmentStoreOpEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Initial Layout", initialLayoutSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkImageLayoutEnumList); n++)
		{
			bool is_selected = (initialLayoutSelection == VulkanMenu::VkImageLayoutEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkImageLayoutEnumList[n], is_selected))
			{
				initialLayoutSelection = VulkanMenu::VkImageLayoutEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Final Layout", finalLayoutSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkImageLayoutEnumList); n++)
		{
			bool is_selected = (finalLayoutSelection == VulkanMenu::VkImageLayoutEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkImageLayoutEnumList[n], is_selected))
			{
				finalLayoutSelection = VulkanMenu::VkImageLayoutEnumList[n];
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

void AddTextureAttachmentMenu::SaveTextureAttachmentDescription(nlohmann::json& json)
{
	json["loadOp"] = VulkanMenu::VkAttachmentLoadOpConverter::SelectionToEnum(loadOpSelection);
	json["storeOp"] = VulkanMenu::VkAttachmentStoreOpConverter::SelectionToEnum(storeOpSelection);
	json["stencilLoadOp"] = VulkanMenu::VkAttachmentLoadOpConverter::SelectionToEnum(stencilLoadOpSelection);
	json["stencilStoreOp"] = VulkanMenu::VkAttachmentStoreOpConverter::SelectionToEnum(stencilStoreOpSelection);
	json["initialLayout"] = VulkanMenu::VkImageLayoutConverter::SelectionToEnum(initialLayoutSelection);
	json["finalLayout"] = VulkanMenu::VkImageLayoutConverter::SelectionToEnum(finalLayoutSelection);
}
