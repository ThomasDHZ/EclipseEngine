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
	ImGui::InputFloat2("Texture Resolution", &TextureResolution[0]);

	if (ImGui::BeginCombo("Texture Format:", TextureFormatSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VKEnumToList::VkFormatEnumList); n++)
		{
			bool is_selected = (TextureFormatSelection == VKEnumToList::VkFormatEnumList[n]);
			if (ImGui::Selectable(VKEnumToList::VkFormatEnumList[n], is_selected))
			{
				TextureFormatSelection = VKEnumToList::VkFormatEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Multi Sampling", MulitSamplerEnumSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VKEnumToList::VkSampleCountFlagBitsEnumList); n++)
		{
			bool is_selected = (MulitSamplerEnumSelection == VKEnumToList::VkSampleCountFlagBitsEnumList[n]);
			if (ImGui::Selectable(VKEnumToList::VkSampleCountFlagBitsEnumList[n], is_selected))
			{
				MulitSamplerEnumSelection = VKEnumToList::VkSampleCountFlagBitsEnumList[n];
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

VkAttachmentDescription AddTextureAttachmentMenu::GetTextureAttachmentDescription()
{
	VkAttachmentDescription RenderedColorTextureAttachment = {};
	RenderedColorTextureAttachment.format = VKEnumToList::VkFormatConverter::SelectionToEnum(TextureFormatSelection);
	RenderedColorTextureAttachment.samples = VKEnumToList::VkSampleCountFlagBitsConverter::SelectionToEnum(MulitSamplerEnumSelection);
	RenderedColorTextureAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	RenderedColorTextureAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	RenderedColorTextureAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	RenderedColorTextureAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	RenderedColorTextureAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	RenderedColorTextureAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	return RenderedColorTextureAttachment;
}
