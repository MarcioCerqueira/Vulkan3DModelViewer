#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

#include "LogicalDeviceInfoModel.h"

class LogicalDevice
{
public:
	LogicalDevice(const LogicalDeviceInfoModel& logicalDeviceInfo);

private:
	vk::Device vulkanLogicalDevice; 
};