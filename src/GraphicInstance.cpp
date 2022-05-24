#include "GraphicInstance.h"

GraphicInstance::GraphicInstance(const std::string& applicationName)
{
	createVulkanInstance(applicationName);
}

GraphicInstance::~GraphicInstance()
{
	vulkanInstance.destroy();
}

void GraphicInstance::createVulkanInstance(const std::string& applicationName)
{
	uint32_t glfwExtensionCount{ 0 };
	const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };
	
	const vk::ApplicationInfo applicationInfo{
		.pApplicationName = applicationName.c_str(),
		.applicationVersion = 1,
		.pEngineName = "No Engine",
		.engineVersion = 1,
		.apiVersion = VK_API_VERSION_1_0
	};
	const vk::InstanceCreateInfo createInfo{ 
		.flags = {}, 
		.pApplicationInfo = &applicationInfo, 
		.enabledLayerCount = validationLayer.getEnabledLayerCount(), 
		.ppEnabledLayerNames = validationLayer.getEnabledLayerNames(), 
		.enabledExtensionCount = glfwExtensionCount, 
		.ppEnabledExtensionNames = glfwExtensions 
	};

	vulkanInstance = vk::createInstance(createInfo);
}

vk::Instance GraphicInstance::getVulkanInstance() const noexcept
{
	return vulkanInstance;
}

void GraphicInstance::selectPhysicalDevice(const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize)
{
	const std::vector<vk::PhysicalDevice> vulkanPhysicalDevices{ vulkanInstance.enumeratePhysicalDevices() };
	physicalDevice.pick(vulkanPhysicalDevices, vulkanWindowSurface, framebufferSize);
}

void GraphicInstance::createLogicalDevice(const vk::SurfaceKHR& vulkanWindowSurface)
{
	logicalDevice = physicalDevice.createLogicalDevice(validationLayer, vulkanWindowSurface);
}