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

VkDebugUtilsMessengerEXT VulkanDebugger::DebugMessenger;
VkDebugUtilsMessengerCreateInfoEXT VulkanDebugger::DebugUtilsMessengerCreateInfoEXT;

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
	/*if (!RayTracingFeatureCompatible)
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
				RayTracingFeatureCompatible = true;
				DeviceExtensions.emplace_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
				DeviceExtensions.emplace_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
			}
			else
			{
				RayTracingFeatureCompatible = false;
			}
		}
		else
		{
			std::cout << "GPU/MotherBoard isn't ray tracing compatible." << std::endl;
		}
	}*/

	//return RayTracingFeatureCompatible;

	return true;
}

VulkanRenderer::VulkanRenderer(Window& window)
{
	//ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

	//DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_KHR_MAINTENANCE3_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
	//DeviceExtensions.emplace_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);

	GetBasicExtentions();

	VkApplicationInfo VulkanApplicationInfo{};
	VulkanApplicationInfo.pApplicationName = "Vulkan Graphics";
	VulkanApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInfo.pEngineName = "No Engine";
	VulkanApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo InstanceCreateInfo{};
	InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceCreateInfo.pApplicationInfo = &VulkanApplicationInfo;
	InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	InstanceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();

#ifdef NDEBUG
	InstanceCreateInfo.enabledLayerCount = 0;
	InstanceCreateInfo.pNext = nullptr;
#else
	VulkanDebugger::SetUpDebuggerInfo();


	std::vector<VkValidationFeatureEnableEXT> enabledList = { VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT, VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT, VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT };
	std::vector<VkValidationFeatureDisableEXT> disabledList = { VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT, VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT, VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT, VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT };

	VkValidationFeaturesEXT ValidationFeatures{};
	ValidationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	ValidationFeatures.disabledValidationFeatureCount = static_cast<uint32_t>(enabledList.size());
	ValidationFeatures.enabledValidationFeatureCount = static_cast<uint32_t>(disabledList.size());
	ValidationFeatures.pEnabledValidationFeatures = enabledList.data();
	ValidationFeatures.pDisabledValidationFeatures = disabledList.data();
	ValidationFeatures.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&VulkanDebugger::DebugUtilsMessengerCreateInfoEXT;

	InstanceCreateInfo.enabledLayerCount = static_cast<unsigned int>(ValidationLayers.size());
	InstanceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	InstanceCreateInfo.pNext = &ValidationFeatures;
#endif

	if(vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance) != VK_SUCCESS)
	{ 
		throw std::runtime_error("Failed to start vulkan instance."); 
	}

	//VulkanDebugger::SetUpDebugger(Instance);
	auto a = glfwCreateWindowSurface(Instance, window.GetWindowPtr(), nullptr, &Surface);
	if (glfwCreateWindowSurface(Instance, window.GetWindowPtr(), nullptr, &Surface) != VK_SUCCESS)
	{ 
		throw std::runtime_error("Failed to create window surface."); 
	}
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
