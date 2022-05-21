#include "PhysicalDevice.h"

void PhysicalDevice::pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface)
{
	checkVulkanSupport(vulkanPhysicalDevices);
	std::multimap<int, vk::PhysicalDevice> mostSuitableDevices{ rateMostSuitablePhysicalDevices(vulkanPhysicalDevices, vulkanWindowSurface) };
	vulkanPhysicalDevice = selectMostSuitablePhysicalDevice(mostSuitableDevices);
	std::cout << "Selected physical device: " << vulkanPhysicalDevice.getProperties().deviceName << std::endl;
}

void PhysicalDevice::checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const
{
	if (vulkanPhysicalDevices.size() == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
}

std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	for (const auto& device : vulkanPhysicalDevices)
	{
		int score{ rateSuitability(device, vulkanWindowSurface) };
		mostSuitablePhysicalDevices.insert(std::make_pair(score, device));
	}
	return mostSuitablePhysicalDevices;
}

int PhysicalDevice::rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	int score{ 0 };
	score += rateSuitabilityByPhysicalDeviceType(vulkanPhysicalDevice);
	score += rateSuitabilityByQueueFamilyProperties(vulkanPhysicalDevice, vulkanWindowSurface);
	score *= rateSuitabilityByDeviceExtensionSupport(vulkanPhysicalDevice);
	return score;
}

int PhysicalDevice::rateSuitabilityByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const
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

int PhysicalDevice::rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	int score{ 0 };
	QueueFamilyIndicesModel queueFamilyIndices = computeQueueFamilyIndices(vulkanPhysicalDevice, vulkanWindowSurface);
	score += 1000 * static_cast<int>(queueFamilyIndices.graphicsFamilyIndex.has_value());
	score += 1000 * static_cast<int>(queueFamilyIndices.presentFamilyIndex.has_value());
	return score;
}

QueueFamilyIndicesModel PhysicalDevice::computeQueueFamilyIndices(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	QueueFamilyIndicesModel queueFamilyIndices;
	queueFamilyIndices.graphicsFamilyIndex = retrieveValidGraphicsFamilyIndex(vulkanPhysicalDevice);
	queueFamilyIndices.presentFamilyIndex = retrieveValidPresentFamilyIndex(vulkanPhysicalDevice, vulkanWindowSurface);
	return queueFamilyIndices;
}

int PhysicalDevice::rateSuitabilityByDeviceExtensionSupport(const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	const std::vector<vk::ExtensionProperties> availableExtensions = vulkanPhysicalDevice.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(vulkanDeviceExtensions.begin(), vulkanDeviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	return static_cast<int>(requiredExtensions.empty());
}

std::optional<uint32_t> PhysicalDevice::retrieveValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	int queueFamilyIndex{ 0 };
	std::optional<uint32_t> graphicsFamilyIndex;
	const std::vector<vk::QueueFamilyProperties> queueFamilies{ vulkanPhysicalDevice.getQueueFamilyProperties() };
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			graphicsFamilyIndex = queueFamilyIndex;
			break;
		}
		++queueFamilyIndex;
	}
	return graphicsFamilyIndex;
}

std::optional<uint32_t> PhysicalDevice::retrieveValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	std::optional<uint32_t> presentFamilyIndex;
	size_t queueFamilySize{ vulkanPhysicalDevice.getQueueFamilyProperties().size() };
	for (int queueFamilyIndex = 0; queueFamilyIndex < queueFamilySize; ++queueFamilyIndex)
	{
		if(vulkanPhysicalDevice.getSurfaceSupportKHR(queueFamilyIndex, vulkanWindowSurface))
		{
			presentFamilyIndex = queueFamilyIndex;
			break;
		}
	}
	return presentFamilyIndex;
}

vk::PhysicalDevice PhysicalDevice::selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const
{
	if (mostSuitablePhysicalDevices.rbegin()->first > 0)
	{
		return mostSuitablePhysicalDevices.rbegin()->second;
	}
	else
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

std::unique_ptr<LogicalDevice> PhysicalDevice::createLogicalDevice(uint32_t enabledLayerCount, 
	char* const* enabledLayerNames, const vk::SurfaceKHR& vulkanWindowSurface)
{
	const LogicalDeviceInfoModel logicalDeviceInfoModel{
		.queueFamilyIndices = computeQueueFamilyIndices(vulkanPhysicalDevice, vulkanWindowSurface),
		.vulkanPhysicalDevice = vulkanPhysicalDevice,
		.vulkanDeviceExtensions = vulkanDeviceExtensions,
		.enabledLayerCount = enabledLayerCount,
		.enabledLayerNames = enabledLayerNames
	};
	return std::make_unique<LogicalDevice>(logicalDeviceInfoModel);
}
