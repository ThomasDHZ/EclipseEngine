#pragma once
#include "VulkanRenderer.h"
#include "VulkanMenu.h"
#include "JsonConverter.h"
#include "ConstMeshInfo.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class AddPipelineMenu
{
private:
	const char* z = "Default";
	std::string pipelineListcharstring;
	std::vector<const char*> pipelineListchar;

public:
	AddPipelineMenu();
	~AddPipelineMenu();

	void ImGuiUpdate(std::string& ColorBlendingName, int id);
	void SavePipeline(nlohmann::json& json);
};

