#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include "WindowSize.h"

struct PhysicalDeviceSuitabilityRaterInfo
{
	vk::PhysicalDevice vulkanPhysicalDevice;
	vk::SurfaceKHR vulkanWindowSurface;
	std::vector<const char*> vulkanDeviceExtensions;
	WindowSize framebufferSize;
};