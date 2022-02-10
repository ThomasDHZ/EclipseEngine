#pragma once
#include "VulkanRenderer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"


class InterfaceRenderPass
{
private:
	static VkRenderPass RenderPass;
	static VkDescriptorPool ImGuiDescriptorPool;
	static VkCommandPool ImGuiCommandPool;

	static std::vector<VkFramebuffer> SwapChainFramebuffers;

	static void CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(VulkanRenderer::GetDevice(), &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

	static void CreateRendererFramebuffers()
    {
        SwapChainFramebuffers.resize(3);

        for (size_t i = 0; i < 3; i++) {
            std::array<VkImageView, 1> attachments = {
                VulkanRenderer::GetSwapChainImageViews()[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = RenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = VulkanRenderer::GetSwapChainResolution().width;
            framebufferInfo.height = VulkanRenderer::GetSwapChainResolution().height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(VulkanRenderer::GetDevice(), &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		printf("VkResult %d\n", err);
		if (err < 0)
			abort();
	}

public:

	static std::vector<VkCommandBuffer> ImGuiCommandBuffers;

	static void StartUp()
    {
        CreateRenderPass();
        CreateRendererFramebuffers();

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = VulkanRenderer::GetInstance();
        init_info.PhysicalDevice = VulkanRenderer::GetPhysicalDevice();
        init_info.Device = VulkanRenderer::GetDevice();
        init_info.QueueFamily = 0;
        init_info.Queue = VulkanRenderer::GetGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.Allocator = nullptr;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;


        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = init_info.QueueFamily;

        if (vkCreateCommandPool(init_info.Device, &poolInfo, nullptr, &ImGuiCommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics command pool!");
        }

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(init_info.Device, &pool_info, nullptr, &ImGuiDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }


        ImGuiCommandBuffers.resize(3);
        for (size_t i = 0; i < 3; i++)
        {
            VkCommandBufferAllocateInfo allocInfo2{};
            allocInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo2.commandPool = ImGuiCommandPool;
            allocInfo2.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo2.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(init_info.Device, &allocInfo2, &ImGuiCommandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate command buffers!");
            }
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(Window::GetWindowPtr(), true);
        init_info.DescriptorPool = ImGuiDescriptorPool;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, RenderPass);

        VkCommandBuffer command_buffer = VulkanRenderer::BeginSingleTimeCommands(ImGuiCommandPool);
        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
        VulkanRenderer::EndSingleTimeCommands(command_buffer, ImGuiCommandPool);
    }

	static void Draw()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if (vkBeginCommandBuffer(ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderPass;
        renderPassInfo.framebuffer = SwapChainFramebuffers[VulkanRenderer::GetImageIndex()];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = VulkanRenderer::GetSwapChainResolution();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);
        vkCmdEndRenderPass(ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

        if (vkEndCommandBuffer(ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

	static void RebuildSwapChain()
    {
        vkDestroyRenderPass(VulkanRenderer::GetDevice(), RenderPass, nullptr);
        RenderPass = VK_NULL_HANDLE;

        for (auto& framebuffer : SwapChainFramebuffers)
        {
            vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
            framebuffer = VK_NULL_HANDLE;
        }

        CreateRenderPass();
        CreateRendererFramebuffers();
    }

	static void Destroy()
    {
        vkDestroyRenderPass(VulkanRenderer::GetDevice(), RenderPass, nullptr);
        RenderPass = VK_NULL_HANDLE;

        for (auto& framebuffer : SwapChainFramebuffers)
        {
            vkDestroyFramebuffer(VulkanRenderer::GetDevice(), framebuffer, nullptr);
            framebuffer = VK_NULL_HANDLE;
        }

        vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), ImGuiDescriptorPool, nullptr);
        vkFreeCommandBuffers(VulkanRenderer::GetDevice(), ImGuiCommandPool, 1, &ImGuiCommandBuffers[0]);
        vkDestroyCommandPool(VulkanRenderer::GetDevice(), ImGuiCommandPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

	VkRenderPass GetRenderPass() { return RenderPass; }
};
