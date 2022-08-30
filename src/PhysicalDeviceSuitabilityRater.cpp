#include "PhysicalDeviceSuitabilityRater.h"

const int PhysicalDeviceSuitabilityRater::rate(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	int score{ 0 };
	score += rateByPhysicalDeviceType(physicalDeviceSuitabilityRaterInfo);
	score += rateByQueueFamilyProperties(physicalDeviceSuitabilityRaterInfo);
	score *= rateByDeviceExtensionSupport(physicalDeviceSuitabilityRaterInfo);
	score *= rateBySwapChainProperties(physicalDeviceSuitabilityRaterInfo);
	score *= rateByDeviceSupportedFeatures(physicalDeviceSuitabilityRaterInfo);
	return score;
}

const int PhysicalDeviceSuitabilityRater::rateByPhysicalDeviceType(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	return physicalDeviceTypeRater.at(physicalDeviceSuitabilityRaterInfo.vulkanPhysicalDevice.getProperties().deviceType);
}

const int PhysicalDeviceSuitabilityRater::rateByQueueFamilyProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	int score{ 0 };
	const QueueFamilyIndices queueFamilyIndices{ 
		physicalDeviceSuitabilityRaterInfo.vulkanPhysicalDevice,
		physicalDeviceSuitabilityRaterInfo.vulkanWindowSurface
	};
	score += 1000 * static_cast<int>(queueFamilyIndices.getGraphicsFamilyIndex().has_value());
	score += 1000 * static_cast<int>(queueFamilyIndices.getPresentFamilyIndex().has_value());
	return score;
}

const int PhysicalDeviceSuitabilityRater::rateByDeviceExtensionSupport(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	const std::vector<vk::ExtensionProperties> availableExtensions = physicalDeviceSuitabilityRaterInfo.vulkanPhysicalDevice.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(physicalDeviceSuitabilityRaterInfo.vulkanDeviceExtensions.begin(), physicalDeviceSuitabilityRaterInfo.vulkanDeviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	return static_cast<int>(requiredExtensions.empty());
}

const int PhysicalDeviceSuitabilityRater::rateBySwapChainProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	return SwapChain::isValid(physicalDeviceSuitabilityRaterInfo.vulkanPhysicalDevice, physicalDeviceSuitabilityRaterInfo.vulkanWindowSurface);
}

const int PhysicalDeviceSuitabilityRater::rateByDeviceSupportedFeatures(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const
{
	return static_cast<int>(physicalDeviceSuitabilityRaterInfo.vulkanPhysicalDevice.getFeatures().samplerAnisotropy);
}