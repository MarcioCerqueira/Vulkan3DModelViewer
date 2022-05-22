#include "PhysicalDeviceSuitabilityRater.h"

int PhysicalDeviceSuitabilityRater::rate(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const std::vector<const char*> vulkanDeviceExtensions, const WindowSize& framebufferSize) const
{
	int score{ 0 };
	score += rateByPhysicalDeviceType(vulkanPhysicalDevice);
	score += rateByQueueFamilyProperties(vulkanPhysicalDevice, vulkanWindowSurface);
	score *= rateByDeviceExtensionSupport(vulkanPhysicalDevice, vulkanDeviceExtensions);
	score *= rateBySwapChainProperties(vulkanPhysicalDevice, vulkanWindowSurface, framebufferSize);
	return score;
}

int PhysicalDeviceSuitabilityRater::rateByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	int score{ 0 };
	switch (vulkanPhysicalDevice.getProperties().deviceType)
	{
	case vk::PhysicalDeviceType::eDiscreteGpu:
		score = 1000;
		break;
	case vk::PhysicalDeviceType::eIntegratedGpu:
		score = 500;
		break;
	case vk::PhysicalDeviceType::eCpu:
		score = 100;
		break;
	default:
		score = 0;
		break;
	}
	return score;
}

int PhysicalDeviceSuitabilityRater::rateByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	int score{ 0 };
	const QueueFamilyIndices queueFamilyIndices{ vulkanPhysicalDevice, vulkanWindowSurface };
	score += 1000 * static_cast<int>(queueFamilyIndices.getGraphicsFamilyIndex().has_value());
	score += 1000 * static_cast<int>(queueFamilyIndices.getPresentFamilyIndex().has_value());
	return score;
}

int PhysicalDeviceSuitabilityRater::rateByDeviceExtensionSupport(const vk::PhysicalDevice& vulkanPhysicalDevice, const std::vector<const char*> vulkanDeviceExtensions) const
{
	const std::vector<vk::ExtensionProperties> availableExtensions = vulkanPhysicalDevice.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(vulkanDeviceExtensions.begin(), vulkanDeviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	return static_cast<int>(requiredExtensions.empty());
}

int PhysicalDeviceSuitabilityRater::rateBySwapChainProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize) const
{
	SwapChain swapChain(vulkanPhysicalDevice, vulkanWindowSurface, framebufferSize);
	return static_cast<int>(swapChain.isValid());
}