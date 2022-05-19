#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const std::optional<uint32_t>& graphicsFamilyIndex, const std::optional<uint32_t>& presentFamilyIndex) : graphicsFamilyIndex(graphicsFamilyIndex), presentFamilyIndex(presentFamilyIndex)
{
}

void LogicalDevice::create(const vk::PhysicalDevice& vulkanPhysicalDevice, const uint32_t enabledLayerCount, const char* const* enabledLayerNames)
{
	const float queuePriority{ 1.0f };
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyIndex.value(), presentFamilyIndex.value() };
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		vk::DeviceQueueCreateInfo logicalDeviceQueueCreateInfo{ .sType = vk::StructureType::eDeviceQueueCreateInfo, .queueFamilyIndex = queueFamily, .queueCount = 1, .pQueuePriorities = &queuePriority };
		queueCreateInfos.push_back(logicalDeviceQueueCreateInfo);
	}
	
	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	const vk::DeviceCreateInfo logicalDeviceCreateInfo{ .sType = vk::StructureType::eDeviceCreateInfo, .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()), .pQueueCreateInfos = queueCreateInfos.data(), .enabledLayerCount = enabledLayerCount, .ppEnabledLayerNames = enabledLayerNames, .enabledExtensionCount = 0, .pEnabledFeatures = &physicalDeviceFeatures};
	vulkanLogicalDevice = vulkanPhysicalDevice.createDevice(logicalDeviceCreateInfo);
}