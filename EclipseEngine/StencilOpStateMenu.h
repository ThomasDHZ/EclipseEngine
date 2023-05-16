#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include <string>
#include <vector>
#include "JsonGraphicsPipeline.h"

class StencilOpStateMenu
{
	private:
        VkStencilOp    failOp;
        VkStencilOp    passOp;
        VkStencilOp    depthFailOp;
        VkCompareOp    compareOp;
        uint32_t       compareMask;
        uint32_t       writeMask;
        uint32_t       reference;
	public:
		StencilOpStateMenu();
		~StencilOpStateMenu();
};

