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
	const vk::ApplicationInfo applicationInfo{ createApplicationInfo(applicationName) };
	const vk::InstanceCreateInfo createInfo{ createVulkanInstanceInfo(applicationInfo) };
	vulkanInstance = vk::createInstance(createInfo);
}

vk::ApplicationInfo GraphicInstance::createApplicationInfo(const std::string& applicationName) const
{
	return vk::ApplicationInfo{
		.pApplicationName = applicationName.c_str(),
		.applicationVersion = 1,
		.pEngineName = "No Engine",
		.engineVersion = 1,
		.apiVersion = VK_API_VERSION_1_0
	};
}

vk::InstanceCreateInfo GraphicInstance::createVulkanInstanceInfo(const vk::ApplicationInfo& applicationInfo) const
{
	uint32_t glfwExtensionCount{ 0 };
	const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };
	return vk::InstanceCreateInfo{
		.flags = {},
		.pApplicationInfo = &applicationInfo,
		.enabledLayerCount = validationLayer.getEnabledLayerCount(),
		.ppEnabledLayerNames = validationLayer.getEnabledLayerNames(),
		.enabledExtensionCount = glfwExtensionCount,
		.ppEnabledExtensionNames = glfwExtensions
	};
}

vk::Instance GraphicInstance::getVulkanInstance() const noexcept
{
	return vulkanInstance;
}

vk::Device GraphicInstance::getVulkanLogicalDevice() const
{
	return logicalDevice->getVulkanLogicalDevice();
}

void GraphicInstance::selectPhysicalDevice(const vk::SurfaceKHR& vulkanWindowSurface)
{
	const std::vector<vk::PhysicalDevice> vulkanPhysicalDevices{ vulkanInstance.enumeratePhysicalDevices() };
	physicalDevice.pick(vulkanPhysicalDevices, vulkanWindowSurface);
}

void GraphicInstance::createLogicalDevice(const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize)
{
	logicalDevice = physicalDevice.createLogicalDevice(validationLayer, vulkanWindowSurface, framebufferSize);
}