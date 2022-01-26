#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>

Window* Window::window;
GLFWwindow* Window::GLFWindow;
uint32_t Window::Width;
uint32_t Window::Height;
bool Window::FramebufferResized;

VkPhysicalDeviceFeatures GPULimitsandFeatures::PhysicalDeviceFeatures;
VkPhysicalDeviceProperties GPULimitsandFeatures::PhysicalDeviceProperties;
VkPhysicalDeviceLimits GPULimitsandFeatures::PhysicalDeviceLimits;
VkPhysicalDeviceAccelerationStructureFeaturesKHR GPULimitsandFeatures::AccelerationStructureFeatures;
VkPhysicalDeviceRayTracingPipelineFeaturesKHR GPULimitsandFeatures::RayTracingPipelineFeatures;

std::unique_ptr<GameController> GameController::Controller = nullptr;
GLFWgamepadstate GameController::ControllerState;

std::unique_ptr<Mouse> Mouse::mouse = nullptr;
double Mouse::ScrollX = 0;
double Mouse::ScrollY = 0;
double Mouse::XPos = 0;
double Mouse::YPos = 0;
double Mouse::LastX = 0;
double Mouse::LastY = 0;
bool   Mouse::MouseButtonPressed[3];
bool   Mouse::IsDragging = false;

std::unique_ptr<Keyboard> Keyboard::keyboard = nullptr;
bool Keyboard::KeyPressed[350];

//VkDebugUtilsMessengerEXT VulkanDebugger::DebugMessenger;
//VkDebugUtilsMessengerCreateInfoEXT VulkanDebugger::DebugUtilsMessengerCreateInfoEXT;

int VulkanRenderer::GraphicsFamily = -1;
int VulkanRenderer::PresentFamily = -1;
uint32_t VulkanRenderer::ImageIndex = 0;
uint32_t VulkanRenderer::CMDIndex = 0;
bool VulkanRenderer::RayTracingFeature = false;

std::vector<const char*> VulkanRenderer::ValidationLayers;
std::vector<const char*> VulkanRenderer::DeviceExtensions;
std::vector<std::string> VulkanRenderer::FeatureList;

std::vector<VkFence> VulkanRenderer::InFlightFences;
std::vector<VkSemaphore> VulkanRenderer::AcquireImageSemaphores;
std::vector<VkSemaphore> VulkanRenderer::PresentImageSemaphores;
VulkanDebugger VulkanRenderer::VulkanDebug;
VulkanSwapChain VulkanRenderer::SwapChain;

VkInstance VulkanRenderer::Instance = VK_NULL_HANDLE;
VkDevice VulkanRenderer::Device = VK_NULL_HANDLE;
VkPhysicalDevice VulkanRenderer::PhysicalDevice = VK_NULL_HANDLE;
VkSurfaceKHR VulkanRenderer::Surface = VK_NULL_HANDLE;
VkQueue VulkanRenderer::GraphicsQueue = VK_NULL_HANDLE;
VkQueue VulkanRenderer::PresentQueue = VK_NULL_HANDLE;
VkCommandPool VulkanRenderer::CommandPool = VK_NULL_HANDLE;

PFN_vkGetBufferDeviceAddressKHR VulkanRenderer::vkGetBufferDeviceAddressKHR = VK_NULL_HANDLE;
PFN_vkCreateAccelerationStructureKHR VulkanRenderer::vkCreateAccelerationStructureKHR = VK_NULL_HANDLE;
PFN_vkDestroyAccelerationStructureKHR VulkanRenderer::vkDestroyAccelerationStructureKHR = VK_NULL_HANDLE;
PFN_vkGetAccelerationStructureBuildSizesKHR VulkanRenderer::vkGetAccelerationStructureBuildSizesKHR = VK_NULL_HANDLE;
PFN_vkGetAccelerationStructureDeviceAddressKHR VulkanRenderer::vkGetAccelerationStructureDeviceAddressKHR = VK_NULL_HANDLE;
PFN_vkCmdBuildAccelerationStructuresKHR VulkanRenderer::vkCmdBuildAccelerationStructuresKHR = VK_NULL_HANDLE;
PFN_vkBuildAccelerationStructuresKHR VulkanRenderer::vkBuildAccelerationStructuresKHR = VK_NULL_HANDLE;
PFN_vkCmdTraceRaysKHR VulkanRenderer::vkCmdTraceRaysKHR = VK_NULL_HANDLE;
PFN_vkGetRayTracingShaderGroupHandlesKHR VulkanRenderer::vkGetRayTracingShaderGroupHandlesKHR = VK_NULL_HANDLE;
PFN_vkCreateRayTracingPipelinesKHR VulkanRenderer::vkCreateRayTracingPipelinesKHR = VK_NULL_HANDLE;

std::vector<const char*> VulkanRenderer::GetRequiredExtensions()
{
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return extensions;
	}
}

void VulkanRenderer::FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
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

void VulkanRenderer::CheckRayTracingCompatiblity(VkPhysicalDevice GPUDevice)
{
	if (!RayTracingFeature)
	{
		VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures{};
		AccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR RayTracingPipelineFeatures{};
		RayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		RayTracingPipelineFeatures.pNext = &AccelerationStructureFeatures;

		VkPhysicalDeviceFeatures2 DeviceFeatures2{};
		DeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		DeviceFeatures2.pNext = &RayTracingPipelineFeatures;
		vkGetPhysicalDeviceFeatures2(GPUDevice, &DeviceFeatures2);

		if (RayTracingPipelineFeatures.rayTracingPipeline == VK_TRUE &&
			AccelerationStructureFeatures.accelerationStructure == VK_TRUE)
		{
			if (std::find(FeatureList.begin(), FeatureList.end(), VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) != FeatureList.end() &&
				std::find(FeatureList.begin(), FeatureList.end(), VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) != FeatureList.end())
			{
				RayTracingFeature = true;
				DeviceExtensions.emplace_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
				DeviceExtensions.emplace_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
			}
			else
			{
				RayTracingFeature = false;
			}
		}
		else
		{
			std::cout << "GPU/MotherBoard isn't ray tracing compatible." << std::endl;
		}
	}
}

std::set<std::string> VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice GPUDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, availableExtensions.data());
	for (auto availableExtension : availableExtensions)
	{
		FeatureList.emplace_back(availableExtension.extensionName);
	}

	std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

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

VkPhysicalDeviceFeatures VulkanRenderer::GetPhysicalDeviceFeatures(VkPhysicalDevice GPUDevice)
{
	VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(GPUDevice, &PhysicalDeviceFeatures);
	return PhysicalDeviceFeatures;
}

std::vector<VkSurfaceFormatKHR> VulkanRenderer::GetSurfaceFormatList(VkPhysicalDevice GPUDevice)
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

std::vector<VkPresentModeKHR> VulkanRenderer::GetPresentModeList(VkPhysicalDevice GPUDevice, VkSurfaceKHR Surface)
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

void VulkanRenderer::StartUp()
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

	DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_MAINTENANCE3_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);

	std::vector<const char*> ExtensionList = GetRequiredExtensions();

	VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
	VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

	std::vector<VkValidationFeatureEnableEXT> EnabledList;
	EnabledList.emplace_back(VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT);
	EnabledList.emplace_back(VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT);
	EnabledList.emplace_back(VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT);

	std::vector<VkValidationFeatureDisableEXT> DisabledList;
	DisabledList.emplace_back(VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT);
	DisabledList.emplace_back(VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT);
	DisabledList.emplace_back(VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT);
	DisabledList.emplace_back(VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT);

	VkValidationFeaturesEXT ValidationFeatures{};
	ValidationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	ValidationFeatures.disabledValidationFeatureCount = static_cast<uint32_t>(EnabledList.size());
	ValidationFeatures.enabledValidationFeatureCount = static_cast<uint32_t>(DisabledList.size());
	ValidationFeatures.pEnabledValidationFeatures = EnabledList.data();
	ValidationFeatures.pDisabledValidationFeatures = DisabledList.data();
	ValidationFeatures.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;

	VkApplicationInfo VulkanApplicationInfo{};
	VulkanApplicationInfo.pApplicationName = "Vulkan Graphics";
	VulkanApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInfo.pEngineName = "No Engine";
	VulkanApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo InstanceCreateInfo{};
	InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceCreateInfo.pApplicationInfo = &VulkanApplicationInfo;
	InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(ExtensionList.size());
	InstanceCreateInfo.ppEnabledExtensionNames = ExtensionList.data();

#ifdef NDEBUG
	InstanceCreateInfo.enabledLayerCount = 0;
	InstanceCreateInfo.pNext = nullptr;
#else
	InstanceCreateInfo.enabledLayerCount = static_cast<unsigned int>(ValidationLayers.size());
	InstanceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	InstanceCreateInfo.pNext = &ValidationFeatures;
#endif

	if (vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to start vulkan instance.");
	}

	VulkanDebug.SetUpDebugger(Instance);

	if (glfwCreateWindowSurface(Instance, Window::GetWindowPtr(), nullptr, &Surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface.");
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support.");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());
	for (const auto& gpudevice : devices)
	{
		FindQueueFamilies(gpudevice, Surface);
		std::set<std::string> extensionsNotSupported = CheckDeviceExtensionSupport(gpudevice);
		VkPhysicalDeviceFeatures supportedFeatures = GetPhysicalDeviceFeatures(gpudevice);
		std::vector<VkSurfaceFormatKHR> SurfaceFormatList = GetSurfaceFormatList(gpudevice);
		std::vector<VkPresentModeKHR> PresentModeList = GetPresentModeList(gpudevice, Surface);

		if (GraphicsFamily != -1 &&
			PresentFamily != -1 &&
			SurfaceFormatList.size() != 0 &&
			PresentModeList.size() != 0 &&
			supportedFeatures.samplerAnisotropy)
		{
			CheckRayTracingCompatiblity(gpudevice);
			PhysicalDevice = gpudevice;
		}
	}
	if (PhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a usable GPU.");
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { static_cast<uint32_t>(GraphicsFamily), static_cast<uint32_t>(PresentFamily) };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceBufferDeviceAddressFeatures BufferDeviceAddresFeatures{};
	BufferDeviceAddresFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
	BufferDeviceAddresFeatures.bufferDeviceAddress = VK_TRUE;

	VkPhysicalDeviceRayTracingPipelineFeaturesKHR RayTracingPipelineFeatures{};
	RayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	RayTracingPipelineFeatures.rayTracingPipeline = VK_TRUE;
	RayTracingPipelineFeatures.pNext = &BufferDeviceAddresFeatures;

	VkPhysicalDeviceRayTracingPipelinePropertiesKHR RayTracingPipelineProperties = {};
	RayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
	RayTracingPipelineProperties.pNext = &RayTracingPipelineFeatures;

	VkPhysicalDeviceAccelerationStructureFeaturesKHR RayTracinDeviceProperties = {};
	RayTracinDeviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	RayTracinDeviceProperties.pNext = &RayTracingPipelineProperties;

	VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures{};
	AccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	AccelerationStructureFeatures.accelerationStructure = VK_TRUE;
	AccelerationStructureFeatures.pNext = &RayTracinDeviceProperties;

	VkPhysicalDeviceDescriptorIndexingFeatures PhysicalDeviceDescriptorIndexingFeatures{};
	PhysicalDeviceDescriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	PhysicalDeviceDescriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
	PhysicalDeviceDescriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
	PhysicalDeviceDescriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
	PhysicalDeviceDescriptorIndexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;
	PhysicalDeviceDescriptorIndexingFeatures.pNext = &AccelerationStructureFeatures;

	VkPhysicalDeviceRobustness2FeaturesEXT  PhysicalDeviceRobustness2Features{};
	PhysicalDeviceRobustness2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
	PhysicalDeviceRobustness2Features.nullDescriptor = VK_TRUE;
	PhysicalDeviceRobustness2Features.pNext = &PhysicalDeviceDescriptorIndexingFeatures;

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.fillModeNonSolid = VK_TRUE;
	deviceFeatures.wideLines = VK_TRUE;
	deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
	deviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;

	VkPhysicalDeviceFeatures2 deviceFeatures2{};
	deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	deviceFeatures2.features = deviceFeatures;
	deviceFeatures2.pNext = &PhysicalDeviceRobustness2Features;

	VkPhysicalDeviceVulkan11Features PhysicalDeviceVulkan11Features{};
	PhysicalDeviceVulkan11Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	PhysicalDeviceVulkan11Features.multiview = VK_TRUE;
	PhysicalDeviceVulkan11Features.pNext = &deviceFeatures2;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = nullptr;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = DeviceExtensions.data();
	createInfo.pNext = &PhysicalDeviceVulkan11Features;

#ifdef NDEBUG
	createInfo.enabledLayerCount = 0;
#else
	createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
	createInfo.ppEnabledLayerNames = ValidationLayers.data();
#endif

	if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(Device, GraphicsFamily, 0, &GraphicsQueue);
	vkGetDeviceQueue(Device, PresentFamily, 0, &PresentQueue);

	GPULimitsandFeatures::GetGPULimitsandFeatures(PhysicalDevice);
	SwapChain = VulkanSwapChain(Window::GetWindowPtr(), Device, PhysicalDevice, Surface);

	VkCommandPoolCreateInfo CommandPoolCreateInfo{};
	CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	CommandPoolCreateInfo.queueFamilyIndex = GraphicsFamily;

	if (vkCreateCommandPool(Device, &CommandPoolCreateInfo, nullptr, &CommandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics command pool.");
	}

	InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	AcquireImageSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	PresentImageSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreTypeCreateInfo timelineCreateInfo;
	timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	timelineCreateInfo.pNext = NULL;
	timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_BINARY;
	timelineCreateInfo.initialValue = 0;

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = &timelineCreateInfo;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t x = 0; x < MAX_FRAMES_IN_FLIGHT; x++)
	{
		if (vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &AcquireImageSemaphores[x]) != VK_SUCCESS ||
			vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &PresentImageSemaphores[x]) != VK_SUCCESS ||
			vkCreateFence(Device, &fenceInfo, nullptr, &InFlightFences[x]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create synchronization objects for a frame.");
		}
	}

	vkGetBufferDeviceAddressKHR = reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(Device, "vkGetBufferDeviceAddressKHR"));
	vkCmdBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkCmdBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(Device, "vkCmdBuildAccelerationStructuresKHR"));
	vkBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(Device, "vkBuildAccelerationStructuresKHR"));
	vkCreateAccelerationStructureKHR = reinterpret_cast<PFN_vkCreateAccelerationStructureKHR>(vkGetDeviceProcAddr(Device, "vkCreateAccelerationStructureKHR"));
	vkDestroyAccelerationStructureKHR = reinterpret_cast<PFN_vkDestroyAccelerationStructureKHR>(vkGetDeviceProcAddr(Device, "vkDestroyAccelerationStructureKHR"));
	vkGetAccelerationStructureBuildSizesKHR = reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(vkGetDeviceProcAddr(Device, "vkGetAccelerationStructureBuildSizesKHR"));
	vkGetAccelerationStructureDeviceAddressKHR = reinterpret_cast<PFN_vkGetAccelerationStructureDeviceAddressKHR>(vkGetDeviceProcAddr(Device, "vkGetAccelerationStructureDeviceAddressKHR"));
	vkCmdTraceRaysKHR = reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetDeviceProcAddr(Device, "vkCmdTraceRaysKHR"));
	vkGetRayTracingShaderGroupHandlesKHR = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetDeviceProcAddr(Device, "vkGetRayTracingShaderGroupHandlesKHR"));
	vkCreateRayTracingPipelinesKHR = reinterpret_cast<PFN_vkCreateRayTracingPipelinesKHR>(vkGetDeviceProcAddr(Device, "vkCreateRayTracingPipelinesKHR"));

}

void VulkanRenderer::Update()
{
}

void VulkanRenderer::StartDraw()
{
	CMDIndex = (CMDIndex + 1) % MAX_FRAMES_IN_FLIGHT;

	vkWaitForFences(Device, 1, &InFlightFences[CMDIndex], VK_TRUE, UINT64_MAX);
	vkResetFences(Device, 1, &InFlightFences[CMDIndex]);

	VkResult result = vkAcquireNextImageKHR(Device, SwapChain.GetSwapChain(), UINT64_MAX, AcquireImageSemaphores[CMDIndex], VK_NULL_HANDLE, &ImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RebuildSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image.");
	}
}


void VulkanRenderer::SubmitDraw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	Update();

	VkSubmitInfo SubmitInfo{};
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = &AcquireImageSemaphores[CMDIndex];
	SubmitInfo.pWaitDstStageMask = waitStages;
	SubmitInfo.commandBufferCount = static_cast<uint32_t>(CommandBufferSubmitList.size());
	SubmitInfo.pCommandBuffers = CommandBufferSubmitList.data();
	SubmitInfo.signalSemaphoreCount = 1;
	SubmitInfo.pSignalSemaphores = &PresentImageSemaphores[ImageIndex];
	VkResult QueueSubmit = vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, InFlightFences[CMDIndex]);
	if (QueueSubmit != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer.");
	}

	VkPresentInfoKHR PresentInfoKHR{};
	PresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfoKHR.waitSemaphoreCount = 1;
	PresentInfoKHR.pWaitSemaphores = &PresentImageSemaphores[ImageIndex];
	PresentInfoKHR.swapchainCount = 1;
	PresentInfoKHR.pSwapchains = &SwapChain.Swapchain;
	PresentInfoKHR.pImageIndices = &ImageIndex;
	VkResult result = vkQueuePresentKHR(PresentQueue, &PresentInfoKHR);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RebuildSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to present swap chain image.");
	}
}

void VulkanRenderer::RebuildSwapChain()
{
	for (auto imageView : SwapChain.GetSwapChainImageViews()) {
		vkDestroyImageView(Device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(Device, SwapChain.GetSwapChain(), nullptr);
	SwapChain.RebuildSwapChain(Window::GetWindowPtr(), Device, PhysicalDevice, Surface);
}

void VulkanRenderer::Destroy()
{
	SwapChain.Destroy(Device);

	vkDestroyCommandPool(Device, CommandPool, nullptr);
	CommandPool = VK_NULL_HANDLE;

	for (size_t x = 0; x < MAX_FRAMES_IN_FLIGHT; x++)
	{
		vkDestroySemaphore(Device, AcquireImageSemaphores[x], nullptr);
		vkDestroySemaphore(Device, PresentImageSemaphores[x], nullptr);
		vkDestroyFence(Device, InFlightFences[x], nullptr);

		AcquireImageSemaphores[x] = VK_NULL_HANDLE;
		PresentImageSemaphores[x] = VK_NULL_HANDLE;
		InFlightFences[x] = VK_NULL_HANDLE;
	}

	vkDestroyDevice(Device, nullptr);
	Device = VK_NULL_HANDLE;

	VulkanDebug.CleanUp(Instance);

	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroyInstance(Instance, nullptr);
}

VkCommandBuffer  VulkanRenderer::BeginSingleTimeCommands() {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(Device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

VkCommandBuffer  VulkanRenderer::BeginSingleTimeCommands(VkCommandPool& commandPool) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(Device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void  VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(GraphicsQueue);

	vkFreeCommandBuffers(Device, CommandPool, 1, &commandBuffer);
}

void  VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool& commandPool) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(GraphicsQueue);

	vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
}

uint32_t VulkanRenderer::GetMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

	for (uint32_t x = 0; x < memProperties.memoryTypeCount; x++)
	{
		if ((typeFilter & (1 << x)) && (memProperties.memoryTypes[x].propertyFlags & properties) == properties) {
			return x;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type.");
}
