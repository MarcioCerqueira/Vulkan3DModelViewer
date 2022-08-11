#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct ImageInfo
{
	vk::Device vulkanLogicalDevice;
	vk::PhysicalDevice vulkanPhysicalDevice;
	int width;
	int height;
};