#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

#include "LogicalDeviceInfo.h"

class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceInfo& logicalDeviceInfo);

private:
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceInfo& logicalDeviceInfo) const;
	vk::Device vulkanLogicalDevice; 
};