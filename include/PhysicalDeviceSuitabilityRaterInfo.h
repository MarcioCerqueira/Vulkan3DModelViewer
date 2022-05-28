#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>

struct PhysicalDeviceSuitabilityRaterInfo
{
	vk::PhysicalDevice vulkanPhysicalDevice;
	vk::SurfaceKHR vulkanWindowSurface;
	std::vector<const char*> vulkanDeviceExtensions;
};