#include "AddColorBlendAttachmentState.h"

AddColorBlendAttachmentState::AddColorBlendAttachmentState()
{
}

AddColorBlendAttachmentState::~AddColorBlendAttachmentState()
{
}

void AddColorBlendAttachmentState::ImGuiUpdate(std::string& ColorBlendingName, int id)
{
	ImGui::Separator();
	ImGui::Text(ColorBlendingName.c_str());
	ImGui::Separator();

	ImGui::PushID(id);

	//if (ImGui::BeginListBox("listbox 1"))
	//{
	//	for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkColorComponentFlagBitsEnumList); n++)
	//	{
	//		const bool is_selected = (item_current_idx[0] == n);
	//		if (ImGui::Selectable(VulkanMenu::VkColorComponentFlagBitsEnumList[n], is_selected))
	//		{
	//			for (int x = 0; x < item_current_idx.size(); x++)
	//			{
	//				if (std::find(item_current_idx.begin(), item_current_idx.end(), n) != item_current_idx.end())
	//				{
	//					std::cout << "Element found";
	//				}
	//				else
	//				{
	//					item_current_idx.emplace_back(n);
	//				}
	//			}
	//		}

	//		for (int x = 0; x < item_current_idx.size(); x++)
	//		{
	//			if (item_current_idx[x])
	//			{
	//				ImGui::SetItemDefaultFocus();
	//			}
	//		}
	//	}
	//	ImGui::EndListBox();
	//}


	ImGui::Checkbox("blendEnable", &blendEnable);
	if (blendEnable)
	{
		ImGui::Checkbox("BlendRed", &BlendRed);
		ImGui::Checkbox("BlendGreen", &BlendGreen);
		ImGui::Checkbox("BlendBlue", &BlendBlue);
		ImGui::Checkbox("BlendAlpha", &BlendAlpha);

		if (ImGui::BeginCombo("srcColorBlendFactor", srcColorBlendFactor))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendFactorEnumList); n++)
			{
				bool is_selected = (srcColorBlendFactor == VulkanMenu::VkBlendFactorEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendFactorEnumList[n], is_selected))
				{
					srcColorBlendFactor = VulkanMenu::VkBlendFactorEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("dstColorBlendFactor", dstColorBlendFactor))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendFactorEnumList); n++)
			{
				bool is_selected = (dstColorBlendFactor == VulkanMenu::VkBlendFactorEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendFactorEnumList[n], is_selected))
				{
					dstColorBlendFactor = VulkanMenu::VkBlendFactorEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("colorBlendOp", colorBlendOp))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendOpEnumList); n++)
			{
				bool is_selected = (colorBlendOp == VulkanMenu::VkBlendOpEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendOpEnumList[n], is_selected))
				{
					colorBlendOp = VulkanMenu::VkBlendOpEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("srcAccessMask", srcAlphaBlendFactor))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendFactorEnumList); n++)
			{
				bool is_selected = (srcAlphaBlendFactor == VulkanMenu::VkBlendFactorEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendFactorEnumList[n], is_selected))
				{
					srcAlphaBlendFactor = VulkanMenu::VkBlendFactorEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("dstAccessMask", dstAlphaBlendFactor))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendFactorEnumList); n++)
			{
				bool is_selected = (dstAlphaBlendFactor == VulkanMenu::VkBlendFactorEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendFactorEnumList[n], is_selected))
				{
					dstAlphaBlendFactor = VulkanMenu::VkBlendFactorEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("alphaBlendOp", alphaBlendOp))
		{
			for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkBlendOpEnumList); n++)
			{
				bool is_selected = (alphaBlendOp == VulkanMenu::VkBlendOpEnumList[n]);
				if (ImGui::Selectable(VulkanMenu::VkBlendOpEnumList[n], is_selected))
				{
					alphaBlendOp = VulkanMenu::VkBlendOpEnumList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::PopID();

	ImGui::Separator();
}

void AddColorBlendAttachmentState::SaveAddColorBlendAttachmentState(nlohmann::json& json)
{
	json["blendEnable"] = blendEnable;
	json["BlendRed"] = BlendRed;
	json["BlendGreen"] = BlendGreen;
	json["BlendBlue"] = BlendBlue;
	json["BlendAlpha"] = BlendAlpha;
	json["srcColorBlendFactor"] = VulkanMenu::VkBlendFactorConverter::SelectionToEnum(srcColorBlendFactor);
	json["dstColorBlendFactor"] = VulkanMenu::VkBlendFactorConverter::SelectionToEnum(dstColorBlendFactor);
	json["colorBlendOp"] = VulkanMenu::VkBlendOpConverter::SelectionToEnum(colorBlendOp);
	json["srcAlphaBlendFactor"] = VulkanMenu::VkBlendFactorConverter::SelectionToEnum(srcAlphaBlendFactor);
	json["dstAlphaBlendFactor"] = VulkanMenu::VkBlendFactorConverter::SelectionToEnum(dstAlphaBlendFactor);
	json["alphaBlendOp"] = VulkanMenu::VkBlendOpConverter::SelectionToEnum(alphaBlendOp);
}
