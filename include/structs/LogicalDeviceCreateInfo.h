#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

#include "PhysicalDeviceProperties.h"
#include "QueueFamilyIndices.h"
#include "WindowSize.h"
#include "Model.h"

struct LogicalDeviceCreateInfo
{
	QueueFamilyIndices queueFamilyIndices;
	PhysicalDeviceProperties& physicalDeviceProperties;
	vk::SurfaceKHR vulkanWindowSurface;
	std::vector<const char*> vulkanDeviceExtensions;
	WindowSize framebufferSize;
	uint32_t enabledLayerCount;
	char* const* enabledLayerNames;
	Model model;
};