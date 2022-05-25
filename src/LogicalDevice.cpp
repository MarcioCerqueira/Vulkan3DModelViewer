#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const LogicalDeviceInfo& logicalDeviceInfo)
{
	const std::set<uint32_t> uniqueQueueFamilies = { 
		logicalDeviceInfo.queueFamilyIndices.getGraphicsFamilyIndex().value(),
		logicalDeviceInfo.queueFamilyIndices.getPresentFamilyIndex().value()
	};
	const std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = buildDeviceQueueCreateInfos(uniqueQueueFamilies);
	const vk::DeviceCreateInfo vulkanLogicalDeviceCreateInfo = buildVulkanLogicalDeviceCreateInfo(deviceQueueCreateInfos, logicalDeviceInfo);
	vulkanLogicalDevice = logicalDeviceInfo.vulkanPhysicalDevice.createDevice(vulkanLogicalDeviceCreateInfo);
}

std::vector<vk::DeviceQueueCreateInfo> LogicalDevice::buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const
{
	const float queuePriority{ 1.0f };
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		vk::DeviceQueueCreateInfo logicalDeviceQueueCreateInfo{
			.sType = vk::StructureType::eDeviceQueueCreateInfo,
			.queueFamilyIndex = queueFamily,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};
		queueCreateInfos.push_back(logicalDeviceQueueCreateInfo);
	}
	return queueCreateInfos;
}

vk::DeviceCreateInfo LogicalDevice::buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceInfo& logicalDeviceInfo) const
{
	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	const vk::DeviceCreateInfo vulkanLogicalDeviceCreateInfo{
		.sType = vk::StructureType::eDeviceCreateInfo,
		.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
		.pQueueCreateInfos = deviceQueueCreateInfos.data(),
		.enabledLayerCount = logicalDeviceInfo.enabledLayerCount,
		.ppEnabledLayerNames = logicalDeviceInfo.enabledLayerNames,
		.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceInfo.vulkanDeviceExtensions.size()),
		.ppEnabledExtensionNames = logicalDeviceInfo.vulkanDeviceExtensions.data(),
		.pEnabledFeatures = &physicalDeviceFeatures
	};
	return vulkanLogicalDeviceCreateInfo;
}