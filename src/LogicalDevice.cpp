#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const LogicalDeviceInfoModel& logicalDeviceInfo)
{
	const float queuePriority{ 1.0f };
	std::set<uint32_t> uniqueQueueFamilies = { 
		logicalDeviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
		logicalDeviceInfo.queueFamilyIndices.presentFamilyIndex.value()
	};
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

	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	const vk::DeviceCreateInfo logicalDeviceCreateInfo{
		.sType = vk::StructureType::eDeviceCreateInfo,
		.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
		.pQueueCreateInfos = queueCreateInfos.data(),
		.enabledLayerCount = logicalDeviceInfo.enabledLayerCount,
		.ppEnabledLayerNames = logicalDeviceInfo.enabledLayerNames,
		.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceInfo.vulkanDeviceExtensions.size()),
		.ppEnabledExtensionNames = logicalDeviceInfo.vulkanDeviceExtensions.data(),
		.pEnabledFeatures = &physicalDeviceFeatures
	};
	vulkanLogicalDevice = logicalDeviceInfo.vulkanPhysicalDevice.createDevice(logicalDeviceCreateInfo);
}