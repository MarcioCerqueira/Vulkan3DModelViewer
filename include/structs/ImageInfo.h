#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "PhysicalDeviceProperties.h"

struct ImageInfo
{
	vk::Device vulkanLogicalDevice;
	const PhysicalDeviceProperties& physicalDeviceProperties;
	int width;
	int height;
	vk::Format format;
	vk::ImageUsageFlags usageFlags;
};