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
	LogicalDevice(const LogicalDeviceInfo& logicalDeviceInfo);

private:
	vk::Device vulkanLogicalDevice; 
};