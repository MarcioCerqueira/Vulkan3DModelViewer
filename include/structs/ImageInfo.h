#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "PhysicalDeviceProperties.h"

struct ImageInfo
{
	const vk::Device& vulkanLogicalDevice;
	const PhysicalDeviceProperties& physicalDeviceProperties;
	int width;
	int height;
	uint32_t mipLevels;
	vk::SampleCountFlagBits sampleCount;
	vk::Format format;
	vk::ImageUsageFlags usageFlags;
};