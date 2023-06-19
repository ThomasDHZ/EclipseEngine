#pragma once
#ifndef EclipseGUI_H
#define EclipseGUI_H

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

namespace EclipseGUI
{
	struct InputInt
	{
		InputInt(const char* label, int* v, int step, int step_fast, int min, int max, ImGuiInputTextFlags flags)
		{

		}
	};
};

#endif
