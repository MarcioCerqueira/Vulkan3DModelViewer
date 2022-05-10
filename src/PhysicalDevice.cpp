#include "PhysicalDevice.h"

void PhysicalDevice::pick(const std::vector<vk::PhysicalDevice>& physicalDevices)
{
	checkVulkanSupport(physicalDevices);
	std::multimap<int, vk::PhysicalDevice> mostSuitableDevices = rateMostSuitablePhysicalDevices(physicalDevices);
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

std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& physicalDevices) const noexcept
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	for (const auto& device : physicalDevices)
	{
		int score = rateSuitability(device);
		mostSuitablePhysicalDevices.insert(std::make_pair(score, device));
	}
	return mostSuitablePhysicalDevices;
}

int PhysicalDevice::rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice) const noexcept
{
	int score = 0;
	score += rateSuitabilityByPhysicalDeviceType(vulkanPhysicalDevice);
	score += rateSuitabilityByQueueFamilyProperties(vulkanPhysicalDevice);
	return score;
}

int PhysicalDevice::rateSuitabilityByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const noexcept
{
	int score = 0;
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

int PhysicalDevice::rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice) const noexcept
{
	int score = 0;
	const std::vector<vk::QueueFamilyProperties> queueFamilies = vulkanPhysicalDevice.getQueueFamilyProperties();
	for(const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			score = 1000;
			break;
		}
	}
	return score;
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
