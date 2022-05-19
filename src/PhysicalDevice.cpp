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
	if (retrieveValidGraphicsFamilyIndex(vulkanPhysicalDevice).has_value())
	{
		score += 1000;
	}
	if (retrieveValidPresentFamilyIndex(vulkanPhysicalDevice, vulkanWindowSurface).has_value())
	{
		score += 1000;
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

std::unique_ptr<LogicalDevice> PhysicalDevice::createLogicalDevice(const uint32_t enabledLayerCount, const char* const* enabledLayerNames, const vk::SurfaceKHR& vulkanWindowSurface)
{
	std::optional<uint32_t> validGraphicsFamilyIndex{ retrieveValidGraphicsFamilyIndex(vulkanPhysicalDevice) };
	std::optional<uint32_t> validPresentFamilyIndex{ retrieveValidPresentFamilyIndex(vulkanPhysicalDevice, vulkanWindowSurface) };
	std::unique_ptr<LogicalDevice> logicalDevice{ make_unique<LogicalDevice>(validGraphicsFamilyIndex, validPresentFamilyIndex) };
	logicalDevice->create(vulkanPhysicalDevice, enabledLayerCount, enabledLayerNames);
	return logicalDevice;
}
