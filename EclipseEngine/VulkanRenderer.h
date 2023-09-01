#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <array>
#include <cmath>

#include "PathConsts.h"
#include "Window.h"
#include "VulkanDebugger.h"
#include "GraphicsDevice.h"
#include "VulkanSwapChain.h"
#include "Timer.h"

struct DescriptorSetBindingStruct
{
	uint32_t DescriptorSlotNumber;
	VkShaderStageFlags StageFlags;
	VkDescriptorType DescriptorType;
	VkWriteDescriptorSetAccelerationStructureKHR AccelerationStructureDescriptor;
	std::vector<VkDescriptorImageInfo> TextureDescriptor;
	std::vector<VkDescriptorBufferInfo> BufferDescriptor;
	uint32_t Count;
};

struct DescriptorSetLayoutBindingInfo
{
	uint32_t Binding;
	VkDescriptorType DescriptorType;
	VkShaderStageFlags StageFlags;
	uint32_t Count;
};

class VulkanRenderer
{
private:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

	static VkInstance Instance;
	static VkDevice Device;
	static VkPhysicalDevice PhysicalDevice;
	static VkSurfaceKHR Surface;
	static VkCommandPool CommandPool;

	static uint32_t ImageIndex;
	static uint32_t CMDIndex;

	static std::vector<const char*> ValidationLayers;

	static std::vector<VkFence> InFlightFences;
	static std::vector<VkSemaphore> AcquireImageSemaphores;
	static std::vector<VkSemaphore> PresentImageSemaphores;
	static VulkanDebugger VulkanDebug;
	static VulkanSwapChain SwapChain;
	static Timer FrameTimer;

public:

	static bool UpdateRendererFlag;
	static bool ImGUILayerActive;
	static uint32_t Max2DLayerCount;

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
	static void RebuildSwapChain();
	static VkResult StartDraw();
	static VkResult SubmitDraw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	static void Destroy();

	static VkCommandBuffer  BeginSingleTimeCommands();
	static VkCommandBuffer  BeginSingleTimeCommands(VkCommandPool& commandPool);
	static VkResult  EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	static VkResult  EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool& commandPool);
	static uint32_t GetMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static uint64_t GetBufferDeviceAddress(VkBuffer buffer);

	static VkInstance GetInstance() { return Instance; };
	static VkDevice GetDevice() { return Device; };
	static VkPhysicalDevice GetPhysicalDevice() { return PhysicalDevice; };
	static VkSurfaceKHR GetSurface() { return Surface; };
	static VkQueue GetGraphicsQueue() { return GraphicsDevice::GetGraphicsQueue(); }
	static VkQueue GetPresentQueue() { return GraphicsDevice::GetPresentQueue(); }
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
	static float 	GetFrameTimeDurationSeconds() { return FrameTimer.GetTimerDurationSeconds(); };
	static float 	GetFrameTimeDurationMilliseconds() { return FrameTimer.GetTimerDurationMilliseconds(); };
	static uint32_t GetMax2DLayerCount() { return Max2DLayerCount; }
};