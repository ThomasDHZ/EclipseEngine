#pragma once
#include "VulkanRenderer.h""
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"



class InterfaceRenderPass
{
private:
	VkRenderPass RenderPass;
	VkDescriptorPool ImGuiDescriptorPool;
	VkCommandPool ImGuiCommandPool;

	void CreateRenderPass();
	void CreateRendererFramebuffers();

	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		printf("VkResult %d\n", err);
		if (err < 0)
			abort();
	}

public:
	InterfaceRenderPass();
	~InterfaceRenderPass();

	std::vector<VkCommandBuffer> ImGuiCommandBuffers;
	std::vector<VkFramebuffer> SwapChainFramebuffers;

	void StartUp();
	void Draw();
	void RebuildSwapChain();
	void Destroy();

	VkRenderPass GetRenderPass() { return RenderPass; }
};
