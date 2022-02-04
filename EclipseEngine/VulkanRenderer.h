#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <array>

#include "Window.h"
#include "VulkanDebugger.h"
#include "LimitsandFeatures.h"
#include "VulkanSwapChain.h"
#include "Pixel.h"

class VulkanRenderer
{
private:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

	static VkInstance Instance;
	static VkDevice Device;
	static VkPhysicalDevice PhysicalDevice;
	static VkSurfaceKHR Surface;
	static VkQueue GraphicsQueue;
	static VkQueue PresentQueue;
	static VkCommandPool CommandPool;

	static int GraphicsFamily;
	static int PresentFamily;
	static uint32_t ImageIndex;
	static uint32_t CMDIndex;
	static bool RayTracingFeature;

	static std::vector<const char*> ValidationLayers;
	static std::vector<const char*> DeviceExtensions;
	static std::vector<std::string> FeatureList;

	static std::vector<VkFence> InFlightFences;
	static std::vector<VkSemaphore> AcquireImageSemaphores;
	static std::vector<VkSemaphore> PresentImageSemaphores;
	static VulkanDebugger VulkanDebug;
	static VulkanSwapChain SwapChain;

	static std::set<std::string> CheckDeviceExtensionSupport(VkPhysicalDevice GPUDevice);
	static VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice GPUDevice);
	static std::vector<VkSurfaceFormatKHR> GetSurfaceFormatList(VkPhysicalDevice GPUDevice);
	static std::vector<VkPresentModeKHR> GetPresentModeList(VkPhysicalDevice GPUDevice, VkSurfaceKHR Surface);
	static std::vector<const char*> GetRequiredExtensions();

	static void CheckRayTracingCompatiblity(VkPhysicalDevice GPUDevice);
	static void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

public:

	static bool UpdateRendererFlag;

	static PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	static PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	static PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	static PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	static PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	static PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	static PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	static PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	static PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	static PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;

	static void StartUp();
	static void Update();
	static void RebuildSwapChain();
	static VkResult StartDraw();
	static VkResult SubmitDraw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	static void Destroy();

	static VkCommandBuffer  BeginSingleTimeCommands();
	static VkCommandBuffer  BeginSingleTimeCommands(VkCommandPool& commandPool);
	static VkResult  EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	static VkResult  EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool& commandPool);
	static uint32_t GetMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	static VkInstance GetInstance() { return Instance; };
	static VkDevice GetDevice() { return Device; };
	static VkPhysicalDevice GetPhysicalDevice() { return PhysicalDevice; };
	static VkSurfaceKHR GetSurface() { return Surface; };
	static VkQueue GetGraphicsQueue() { return GraphicsQueue; };
	static VkQueue GetPresentQueue() { return PresentQueue; };
	static VkCommandPool GetCommandPool() { return CommandPool; };
	static uint32_t GetImageIndex() { return ImageIndex; }
	static uint32_t GetCMDIndex() { return CMDIndex; }
	static std::vector<VkImage> GetSwapChainImages() { return SwapChain.GetSwapChainImages(); }
	static std::vector<VkImageView> GetSwapChainImageViews() { return SwapChain.GetSwapChainImageViews(); }
	static VkExtent2D GetSwapChainResolution() { return SwapChain.GetSwapChainResolution(); }
	static glm::ivec2 GetSwapChainResolutionVec2() { return glm::ivec2((int)SwapChain.GetSwapChainResolution().width, (int)SwapChain.GetSwapChainResolution().height); }
	static VkSurfaceFormatKHR GetSwapChainImageFormat() { return SwapChain.GetSwapChainImageFormat(); }
	static uint32_t GetSwapChainMinImageCount() { return SwapChain.GetSwapChainMinImageCount(); }
	static uint32_t GetSwapChainImageCount() { return SwapChain.GetSwapChainImageCount(); }
};