#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>

#include "WindowSize.h"
#include "QueueFamilyIndices.h"
#include "PhysicalDeviceProperties.h"

struct SwapChainCreateInfo
{
	PhysicalDeviceProperties& physicalDeviceProperties;
	vk::SurfaceKHR vulkanWindowSurface;
	WindowSize framebufferSize;
	QueueFamilyIndices queueFamilyIndices;
	vk::Device vulkanLogicalDevice;
	vk::Format depthImageFormat;
};