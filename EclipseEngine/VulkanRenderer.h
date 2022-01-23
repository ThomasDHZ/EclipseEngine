#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <set>

#include "Window.h"
#include "VulkanDebugger.h"
#include "LimitsandFeatures.h"
#include "VulkanSwapChain.h"

class VulkanRenderer
{
private:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
	int GraphicsFamily = -1;
	int PresentFamily = -1;
	bool RayTracingFeature = false;

	std::vector<const char*> ValidationLayers;
	std::vector<const char*> DeviceExtensions;
	std::vector<std::string> FeatureList;

	std::vector<VkFence> InFlightFences;
	std::vector<VkSemaphore> AcquireImageSemaphores;
	std::vector<VkSemaphore> PresentImageSemaphores;
	VulkanDebugger VulkanDebug;
	VulkanSwapChain SwapChain;

	std::set<std::string> CheckDeviceExtensionSupport(VkPhysicalDevice GPUDevice);
	VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice GPUDevice);
	std::vector<VkSurfaceFormatKHR> GetSurfaceFormatList(VkPhysicalDevice GPUDevice);
	std::vector<VkPresentModeKHR> GetPresentModeList(VkPhysicalDevice GPUDevice, VkSurfaceKHR Surface);
	std::vector<const char*> getRequiredExtensions();

	void CheckRayTracingCompatiblity(VkPhysicalDevice GPUDevice);
	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

public:
	static VkInstance Instance;
	static VkDevice Device;
	static VkPhysicalDevice PhysicalDevice;
	static VkSurfaceKHR Surface;
	static VkQueue GraphicsQueue;
	static VkQueue PresentQueue;
	static VkCommandPool CommandPool;

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

	VulkanRenderer(Window& window);
	~VulkanRenderer();
	void StartUp();
	void Update();
	void Destroy();
};