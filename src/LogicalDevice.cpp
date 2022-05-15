#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const vk::PhysicalDevice& vulkanPhysicalDevice, const std::optional<uint32_t>& graphicsFamilyIndex, const uint32_t enabledLayerCount, const char* const* enabledLayerNames)
{
	float queuePriority{ 1.0f };
	vk::DeviceQueueCreateInfo logicalDeviceQueueCreateInfo{ .sType = vk::StructureType::eDeviceQueueCreateInfo, .queueFamilyIndex = graphicsFamilyIndex.value(), .queueCount = 1, .pQueuePriorities = &queuePriority };

	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	const vk::DeviceCreateInfo logicalDeviceCreateInfo{ .sType = vk::StructureType::eDeviceCreateInfo, .queueCreateInfoCount = 1, .pQueueCreateInfos = &logicalDeviceQueueCreateInfo, .enabledLayerCount = enabledLayerCount, .ppEnabledLayerNames = enabledLayerNames, .enabledExtensionCount = 0, .pEnabledFeatures = &physicalDeviceFeatures};

	vulkanLogicalDevice = vulkanPhysicalDevice.createDevice(logicalDeviceCreateInfo);
}