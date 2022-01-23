#pragma once
#include <vulkan/vulkan.h>

class GPULimitsandFeatures
{
private:
	static VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	static VkPhysicalDeviceProperties PhysicalDeviceProperties;
	static VkPhysicalDeviceLimits PhysicalDeviceLimits;
	static VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures;
	static VkPhysicalDeviceRayTracingPipelineFeaturesKHR RayTracingPipelineFeatures;

public:

	static void GetGPULimitsandFeatures(VkPhysicalDevice GPUDevice)
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
	}

	static VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures() { return PhysicalDeviceFeatures; }
	static VkPhysicalDeviceProperties GetPhysicalDeviceProperties() { return PhysicalDeviceProperties; }
	static VkPhysicalDeviceLimits GetPhysicalDeviceFeaPhysicalDeviceLimitstures() { return PhysicalDeviceLimits; }
	static VkPhysicalDeviceAccelerationStructureFeaturesKHR GetAccelerationStructureFeatures() { return AccelerationStructureFeatures; }
	static VkPhysicalDeviceRayTracingPipelineFeaturesKHR GetRayTracingPipelineFeatures() { return RayTracingPipelineFeatures; }
};