#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

#include "QueueFamilyIndices.h"

struct LogicalDeviceInfo
{
	QueueFamilyIndices queueFamilyIndices;
	vk::PhysicalDevice vulkanPhysicalDevice;
	std::vector<const char*> vulkanDeviceExtensions;
	uint32_t enabledLayerCount;
	char* const* enabledLayerNames;
};