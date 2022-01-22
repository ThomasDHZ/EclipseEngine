#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>


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

VkInstance VulkanRenderer::Instance = VK_NULL_HANDLE;
VkDevice VulkanRenderer::Device = VK_NULL_HANDLE;
VkPhysicalDevice VulkanRenderer::PhysicalDevice = VK_NULL_HANDLE;
VkSurfaceKHR VulkanRenderer::Surface = VK_NULL_HANDLE;
VkQueue VulkanRenderer::GraphicsQueue = VK_NULL_HANDLE;
VkQueue VulkanRenderer::PresentQueue = VK_NULL_HANDLE;
VkCommandPool VulkanRenderer::CommandPool = VK_NULL_HANDLE;

//PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR = VK_NULL_HANDLE;
//PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = VK_NULL_HANDLE;
//PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = VK_NULL_HANDLE;
//PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = VK_NULL_HANDLE;
//PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = VK_NULL_HANDLE;
//PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = VK_NULL_HANDLE;
//PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR = VK_NULL_HANDLE;
//PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR = VK_NULL_HANDLE;
//PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR = VK_NULL_HANDLE;
//PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR = VK_NULL_HANDLE;

void VulkanRenderer::StartUpDebugger()
{
}

void VulkanRenderer::GetBasicExtentions()
{
	//uint32_t glfwExtensionCount = 0;
	//const char** glfwExtensions;
	//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	//for (auto extension : extensions)
	//{
	//	DeviceExtensions.emplace_back(extension);
	//}
	//DeviceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

bool VulkanRenderer::CheckRayTracingCompatiblity(VkPhysicalDevice GPUDevice)
{
	//if (!RayTracingFeatureCompatible)
	//{
	//	VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures{};
	//	AccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;

	//	VkPhysicalDeviceRayTracingPipelineFeaturesKHR RayTracingPipelineFeatures{};
	//	RayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	//	RayTracingPipelineFeatures.pNext = &AccelerationStructureFeatures;

	//	VkPhysicalDeviceFeatures2 DeviceFeatures2{};
	//	DeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	//	DeviceFeatures2.pNext = &RayTracingPipelineFeatures;
	//	vkGetPhysicalDeviceFeatures2(GPUDevice, &DeviceFeatures2);

	//	if (RayTracingPipelineFeatures.rayTracingPipeline == VK_TRUE &&
	//		AccelerationStructureFeatures.accelerationStructure == VK_TRUE)
	//	{
	//		if (std::find(FeatureList.begin(), FeatureList.end(), VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) != FeatureList.end() &&
	//			std::find(FeatureList.begin(), FeatureList.end(), VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) != FeatureList.end())
	//		{
	//			RayTracingFeatureCompatible = true;
	//			DeviceExtensions.emplace_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
	//			DeviceExtensions.emplace_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
	//		}
	//		else
	//		{
	//			RayTracingFeatureCompatible = false;
	//		}
	//	}
	//	else
	//	{
	//		std::cout << "GPU/MotherBoard isn't ray tracing compatible." << std::endl;
	//	}
	//}

	//return RayTracingFeatureCompatible;

	return false;
}

std::vector<const char*> VulkanRenderer::getRequiredExtensions()
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

std::set<std::string> VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice GPUDevice)
{
	std::vector<std::string> FeatureList;

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

VulkanRenderer::VulkanRenderer(Window& window)
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

	DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_MAINTENANCE3_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
	DeviceExtensions.emplace_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);

	std::vector<const char*> ExtensionList = getRequiredExtensions();

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

	if (glfwCreateWindowSurface(Instance, window.GetWindowPtr(), nullptr, &Surface) != VK_SUCCESS)
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
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::StartUp()
{
}

void VulkanRenderer::Update()
{
}

void VulkanRenderer::Destroy()
{
}
