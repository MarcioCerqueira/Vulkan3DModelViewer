#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>
#include <optional>

#include "LogicalDevice.h"

class PhysicalDevice
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& physicalDevices);
	void createLogicalDevice(const uint32_t enabledLayerCount, const char* const* enabledLayerNames);

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& physicalDevices) const;
	std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& physicalDevices) const;
	int rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	int rateSuitabilityByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	int rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	std::optional<uint32_t> retrieveValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	vk::PhysicalDevice vulkanPhysicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
};