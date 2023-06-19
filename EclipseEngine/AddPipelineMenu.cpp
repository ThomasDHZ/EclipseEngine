#include "AddPipelineMenu.h"
AddPipelineMenu::AddPipelineMenu()
{
}

AddPipelineMenu::~AddPipelineMenu()
{
}

void AddPipelineMenu::ImGuiUpdate(std::string& ColorBlendingName, int id)
{
	ImGui::Separator();
	ImGui::Text(ColorBlendingName.c_str());
	ImGui::Separator();

	ImGui::PushID(id);

	ImGui::Begin("Pipeline List");

	std::vector<std::string> pipelineListcharstring;
	std::vector<const char*> pipelineListchar;
	pipelineListchar.emplace_back("Default");

	for (const auto& file : std::filesystem::directory_iterator(PathConsts::PipelinePath))
	{
		auto splitFile = file.path().string().substr(PathConsts::PipelinePath.size());
		pipelineListcharstring.emplace_back(splitFile);
	}
	for (auto& list : pipelineListcharstring)
	{
		pipelineListchar.emplace_back(list.c_str());
	}

	int z = 0;
	if (ImGui::ListBox("PipelineSelection", &z, pipelineListchar.data(), pipelineListchar.size()))
	{
	}

	ImGui::PopID();

}

void AddPipelineMenu::SaveAddColorBlendAttachmentState(nlohmann::json& json)
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
