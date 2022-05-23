#pragma once
#include <vulkan/vulkan.h>

class GPULimitsandFeatures
{
private:
	static int GraphicsFamily;
	static int PresentFamily;
	static VkQueue GraphicsQueue;
	static VkQueue PresentQueue;


	static std::vector<std::string> FeatureList;
	static std::vector<const char*> DeviceExtensionList;
	static VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	static VkPhysicalDeviceProperties PhysicalDeviceProperties;
	static VkPhysicalDeviceLimits PhysicalDeviceLimits;
	static VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures;
	static VkPhysicalDeviceRayTracingPipelineFeaturesKHR RayTracingPipelineFeatures;
	static VkSampleCountFlagBits MaxSampleCount;

public:

	static void GetGPULimitsandFeatures(VkDevice& Device, VkPhysicalDevice GPUDevice)
	{
		AccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;

		RayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		RayTracingPipelineFeatures.pNext = &AccelerationStructureFeatures;

		VkPhysicalDeviceFeatures2 PhysicalDeviceFeatures2{};
		PhysicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		PhysicalDeviceFeatures2.pNext = &RayTracingPipelineFeatures;
		vkGetPhysicalDeviceFeatures2(GPUDevice, &PhysicalDeviceFeatures2);
		PhysicalDeviceFeatures = PhysicalDeviceFeatures2.features;

		vkGetPhysicalDeviceProperties(GPUDevice, &PhysicalDeviceProperties);
		PhysicalDeviceLimits = PhysicalDeviceProperties.limits;

		VkSampleCountFlags counts = GPULimitsandFeatures::GetPhysicalDeviceFeaPhysicalDeviceLimits().framebufferColorSampleCounts & GPULimitsandFeatures::GetPhysicalDeviceFeaPhysicalDeviceLimits().framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { MaxSampleCount = VK_SAMPLE_COUNT_2_BIT; }


		vkGetDeviceQueue(Device, GraphicsFamily, 0, &GraphicsQueue);
		vkGetDeviceQueue(Device, PresentFamily, 0, &PresentQueue);
	}

	static std::vector<const char*> GetRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return extensions;
	}

	static void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int x = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				GraphicsFamily = x;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, x, Surface, &presentSupport);

			if (presentSupport) {
				PresentFamily = x;
			}

			if (GraphicsFamily != -1 &&
				PresentFamily != -1)
			{
				break;
			}

			x++;
		}
	}

	static std::vector<VkSurfaceFormatKHR> GetSurfaceFormatList(VkPhysicalDevice GPUDevice, VkSurfaceKHR& Surface)
	{
		uint32_t GPUSurfaceFormatCount;
		std::vector<VkSurfaceFormatKHR> GPUSwapChainFormatCapabilities;

		vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, nullptr);
		if (GPUSurfaceFormatCount != 0)
		{
			GPUSwapChainFormatCapabilities.resize(GPUSurfaceFormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, GPUSwapChainFormatCapabilities.data());
		}

		return GPUSwapChainFormatCapabilities;
	}

	static std::vector<VkPresentModeKHR> GetPresentModeList(VkPhysicalDevice GPUDevice, VkSurfaceKHR Surface)
	{
		uint32_t GPUPresentModeCount;
		std::vector<VkPresentModeKHR> GPUPresentModesList;

		vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, nullptr);
		if (GPUPresentModeCount != 0)
		{
			GPUPresentModesList.resize(GPUPresentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, GPUPresentModesList.data());
		}

		return GPUPresentModesList;
	}

	static std::set<std::string> CheckDeviceExtensionSupport(VkPhysicalDevice GPUDevice)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, availableExtensions.data());
		for (auto availableExtension : availableExtensions)
		{
			FeatureList.emplace_back(availableExtension.extensionName);
		}

		std::set<std::string> requiredExtensions(DeviceExtensionList.begin(), DeviceExtensionList.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		for (auto extension : requiredExtensions)
		{
			std::cout << extension << " is Required." << std::endl;
		}

		return requiredExtensions;
	}

	static VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice GPUDevice)
	{
		VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
		vkGetPhysicalDeviceFeatures(GPUDevice, &PhysicalDeviceFeatures);
		return PhysicalDeviceFeatures;
	}


	static void AddRequriedDeviceExtensions(const char* DeviceExtension)
	{
		DeviceExtensionList.emplace_back(DeviceExtension);
	}

	static VkSampleCountFlagBits GetMaxSampleCount() { return MaxSampleCount; };
	static int GetGraphicsFamily() { return GraphicsFamily; }
	static int GetPresentFamily() { return PresentFamily; }
	static VkQueue GetGraphicsQueue() { return GraphicsQueue; }
	static VkQueue GetPresentQueue() { return PresentQueue; }
	static VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures() { return PhysicalDeviceFeatures; }
	static VkPhysicalDeviceProperties GetPhysicalDeviceProperties() { return PhysicalDeviceProperties; }
	static VkPhysicalDeviceLimits GetPhysicalDeviceFeaPhysicalDeviceLimits() { return PhysicalDeviceLimits; }
	static VkPhysicalDeviceAccelerationStructureFeaturesKHR GetAccelerationStructureFeatures() { return AccelerationStructureFeatures; }
	static VkPhysicalDeviceRayTracingPipelineFeaturesKHR GetRayTracingPipelineFeatures() { return RayTracingPipelineFeatures; }
};