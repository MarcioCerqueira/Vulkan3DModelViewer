#include "PhysicalDevice.h"

void PhysicalDevice::pick(const std::vector<vk::PhysicalDevice>& physicalDevices)
{
	checkVulkanSupport(physicalDevices);
	std::multimap<int, vk::PhysicalDevice> mostSuitableDevices{ rateMostSuitablePhysicalDevices(physicalDevices) };
	vulkanPhysicalDevice = selectMostSuitablePhysicalDevice(mostSuitableDevices);
	std::cout << "Selected physical device: " << vulkanPhysicalDevice.getProperties().deviceName << std::endl;
}

void PhysicalDevice::checkVulkanSupport(const std::vector<vk::PhysicalDevice>& physicalDevices) const
{
	if (physicalDevices.size() == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
}

std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& physicalDevices) const
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	for (const auto& device : physicalDevices)
	{
		int score{ rateSuitability(device) };
		mostSuitablePhysicalDevices.insert(std::make_pair(score, device));
	}
	return mostSuitablePhysicalDevices;
}

int PhysicalDevice::rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	int score{ 0 };
	score += rateSuitabilityByPhysicalDeviceType(vulkanPhysicalDevice);
	score += rateSuitabilityByQueueFamilyProperties(vulkanPhysicalDevice);
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

int PhysicalDevice::rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	int score{ 0 };
	if (retrieveValidGraphicsFamilyIndex(vulkanPhysicalDevice).has_value())
	{
		score = 1000;
	}
	return score;
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

void PhysicalDevice::createLogicalDevice(const uint32_t enabledLayerCount, const char* const* enabledLayerNames)
{
	std::optional<uint32_t> validGraphicsFamilyIndex{ retrieveValidGraphicsFamilyIndex(vulkanPhysicalDevice) };
	logicalDevice = make_unique<LogicalDevice>(vulkanPhysicalDevice, validGraphicsFamilyIndex, enabledLayerCount, enabledLayerNames);
}
