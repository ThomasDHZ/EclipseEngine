#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Window.h"
#include "VulkanDebugger.h"

class VulkanRenderer
{
private:
	std::vector<const char*> ValidationLayers;
	std::vector<const char*> DeviceExtensions;

	std::vector<VkFence> InFlightFences;
	std::vector<VkSemaphore> AcquireImageSemaphores;
	std::vector<VkSemaphore> PresentImageSemaphores;

	void StartUpDebugger();

	void GetBasicExtentions();
	bool CheckRayTracingCompatiblity(VkPhysicalDevice GPUDevice);

public:
	static VkInstance Instance;
	static VkDevice Device;
	static VkPhysicalDevice PhysicalDevice;
	static VkSurfaceKHR Surface;
	static VkQueue GraphicsQueue;
	static VkQueue PresentQueue;
	static VkCommandPool CommandPool;

	//static PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	//static PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	//static PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	//static PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	//static PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	//static PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	//static PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	//static PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	//static PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	//static PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;

	VulkanRenderer(Window& window);
	~VulkanRenderer();
	void StartUp();
	void Update();
	void Destroy();
};