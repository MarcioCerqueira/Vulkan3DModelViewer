#include "GraphicsInstance.h"

GraphicsInstance::GraphicsInstance(const std::string& applicationName)
{
	createVulkanInstance(applicationName);
}

GraphicsInstance::~GraphicsInstance()
{
	logicalDevice.reset();
	windowSurface.reset();
	vulkanInstance.destroy();
}

void GraphicsInstance::createVulkanInstance(const std::string& applicationName)
{
	const vk::ApplicationInfo applicationInfo{ createApplicationInfo(applicationName) };
	const vk::InstanceCreateInfo createInfo{ createVulkanInstanceInfo(applicationInfo) };
	vulkanInstance = vk::createInstance(createInfo);
}

const vk::ApplicationInfo GraphicsInstance::createApplicationInfo(const std::string& applicationName) const
{
	return vk::ApplicationInfo{
		.pApplicationName = applicationName.c_str(),
		.applicationVersion = 1,
		.pEngineName = "No Engine",
		.engineVersion = 1,
		.apiVersion = VK_API_VERSION_1_0
	};
}

const vk::InstanceCreateInfo GraphicsInstance::createVulkanInstanceInfo(const vk::ApplicationInfo& applicationInfo) const
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

void GraphicsInstance::createWindowSurface(GLFWwindow* glfwWindow)
{
	windowSurface = std::make_unique<WindowSurface>(vulkanInstance, glfwWindow);
}

void GraphicsInstance::selectPhysicalDevice()
{
	const std::vector<vk::PhysicalDevice> vulkanPhysicalDevices{ vulkanInstance.enumeratePhysicalDevices() };
	physicalDevice.pick(vulkanPhysicalDevices, windowSurface->getVulkanWindowSurface());
}

void GraphicsInstance::createLogicalDevice(const WindowSize& framebufferSize)
{
	logicalDevice = physicalDevice.createLogicalDevice(validationLayer, windowSurface->getVulkanWindowSurface(), framebufferSize);
}

void GraphicsInstance::createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	logicalDevice->createGraphicsPipeline(shaders);
}

void GraphicsInstance::drawFrame(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents)
{
	logicalDevice->drawFrame(getFramebufferSize, waitEvents);
}

void GraphicsInstance::waitIdle()
{
	logicalDevice->waitIdle();
}

const vk::Device GraphicsInstance::getVulkanLogicalDevice() const
{
	return logicalDevice->getVulkanLogicalDevice();
}