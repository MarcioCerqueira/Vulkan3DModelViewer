#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

#include "LogicalDeviceInfo.h"
#include "SwapChain.h"

class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceInfo& logicalDeviceInfo);
	SwapChain createSwapChain(SwapChainInfo& swapChainInfo) const;

private:
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;

	vk::Device vulkanLogicalDevice; 
};