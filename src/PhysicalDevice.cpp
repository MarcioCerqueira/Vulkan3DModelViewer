#include "PhysicalDevice.h"

void PhysicalDevice::pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize)
{
	checkVulkanSupport(vulkanPhysicalDevices);
	std::multimap<int, vk::PhysicalDevice> mostSuitableDevices{ rateMostSuitablePhysicalDevices(vulkanPhysicalDevices, vulkanWindowSurface, framebufferSize) };
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

std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize) const
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	for (const auto& vulkanPhysicalDevice : vulkanPhysicalDevices)
	{
		PhysicalDeviceSuitabilityRaterInfo physicalDeviceSuitabilityRaterInfo{
			.vulkanPhysicalDevice = vulkanPhysicalDevice,
			.vulkanWindowSurface = vulkanWindowSurface,
			.vulkanDeviceExtensions = vulkanDeviceExtensions,
			.framebufferSize = framebufferSize
		};
		int score{ physicalDeviceSuitabilityRater.rate(physicalDeviceSuitabilityRaterInfo) };
		mostSuitablePhysicalDevices.insert(std::make_pair(score, vulkanPhysicalDevice));
	}
	return mostSuitablePhysicalDevices;
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

std::unique_ptr<LogicalDevice> PhysicalDevice::createLogicalDevice(const ValidationLayer& validationLayer, const vk::SurfaceKHR& vulkanWindowSurface)
{
	const LogicalDeviceInfo logicalDeviceInfo{
		.queueFamilyIndices = QueueFamilyIndices(vulkanPhysicalDevice, vulkanWindowSurface),
		.vulkanPhysicalDevice = vulkanPhysicalDevice,
		.vulkanDeviceExtensions = vulkanDeviceExtensions,
		.enabledLayerCount = validationLayer.getEnabledLayerCount(),
		.enabledLayerNames = validationLayer.getEnabledLayerNames()
	};
	return std::make_unique<LogicalDevice>(logicalDeviceInfo);
}
