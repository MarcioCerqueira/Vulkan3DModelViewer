#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

#include "LogicalDeviceCreateInfo.h"
#include "SwapChain.h"

class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);

private:
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	vk::DeviceQueueCreateInfo buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const;
	vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const;
	void createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);

	vk::Device vulkanLogicalDevice; 
	std::shared_ptr<SwapChain> swapChain;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};