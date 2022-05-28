#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>

#include "WindowSize.h"
#include "QueueFamilyIndices.h"

struct SwapChainCreateInfo
{
	vk::PhysicalDevice vulkanPhysicalDevice; 
	vk::SurfaceKHR vulkanWindowSurface;
	WindowSize framebufferSize;
	QueueFamilyIndices queueFamilyIndices;
	vk::Device vulkanLogicalDevice;
};