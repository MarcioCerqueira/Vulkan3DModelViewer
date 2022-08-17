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

const std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	for (const auto& vulkanPhysicalDevice : vulkanPhysicalDevices)
	{
		PhysicalDeviceSuitabilityRaterInfo physicalDeviceSuitabilityRaterInfo{
			.vulkanPhysicalDevice = vulkanPhysicalDevice,
			.vulkanWindowSurface = vulkanWindowSurface,
			.vulkanDeviceExtensions = vulkanDeviceExtensions
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

std::unique_ptr<LogicalDevice> PhysicalDevice::createLogicalDevice(const LogicalDevicePartialCreateInfo& logicalDevicePartialCreateInfo)
{
	const LogicalDeviceCreateInfo logicalDeviceCreateInfo{
		.queueFamilyIndices = QueueFamilyIndices(vulkanPhysicalDevice, logicalDevicePartialCreateInfo.vulkanWindowSurface),
		.physicalDeviceProperties = *this,
		.vulkanWindowSurface = logicalDevicePartialCreateInfo.vulkanWindowSurface,
		.vulkanDeviceExtensions = vulkanDeviceExtensions,
		.framebufferSize = logicalDevicePartialCreateInfo.framebufferSize,
		.enabledLayerCount = logicalDevicePartialCreateInfo.enabledLayerCount,
		.enabledLayerNames = logicalDevicePartialCreateInfo.enabledLayerNames,
		.model = logicalDevicePartialCreateInfo.model
	};
	return std::make_unique<LogicalDevice>(logicalDeviceCreateInfo);
}

const uint32_t PhysicalDevice::findMemoryType(uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	const vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{ vulkanPhysicalDevice.getMemoryProperties() };
	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if (memoryTypeFilter & (1 << i))
		{
			if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
			{
				return i;
			}
		}
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

const vk::Format PhysicalDevice::findSupportedFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling& tiling, const vk::FormatFeatureFlags& features) const
{
	for (const auto& format : candidates)
	{
		const vk::FormatProperties formatProperties{ vulkanPhysicalDevice.getFormatProperties(format) };
		if (tiling == vk::ImageTiling::eLinear && (formatProperties.linearTilingFeatures & features) == features)
		{
			return format;
		}
		if (tiling == vk::ImageTiling::eOptimal && (formatProperties.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}
	throw std::runtime_error("Failed to find supported features!");
}

const vk::PhysicalDevice PhysicalDevice::getVulkanPhysicalDevice() const
{
	return vulkanPhysicalDevice;
}
