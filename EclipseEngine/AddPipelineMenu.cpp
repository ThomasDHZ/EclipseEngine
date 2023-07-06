#include "AddPipelineMenu.h"
#include "JsonGraphicsPipeline.h"
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

	if (pipelineListchar.size() == 0)
	{
		pipelineListchar.emplace_back("Default");

		for (const auto& file : std::filesystem::directory_iterator(PathConsts::PipelinePath))
		{
			auto splitFile = file.path().string().substr(PathConsts::PipelinePath.size());
			pipelineListcharstring = splitFile;
		}
	/*	for (auto& list : pipelineListcharstring)
		{
			pipelineListchar.emplace_back(list.c_str());
		}*/
	}

	if (ImGui::BeginCombo("uyi", z))
	{
		for (int n = 0; n < pipelineListchar.size(); n++)
		{
			bool is_selected = (z == pipelineListchar[n]);
			if (ImGui::Selectable(pipelineListchar[n], is_selected))
			{
				z = pipelineListchar[n];
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

void AddPipelineMenu::SavePipeline(nlohmann::json& json)
{
	json["Pipeline"] = PipelineRenderTypeConverter::SelectionToEnum(pipelineListcharstring);
}
