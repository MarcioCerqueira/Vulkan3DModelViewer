#pragma once
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

#include "WindowSize.h"
#include "Model.h"

struct LogicalDevicePartialCreateInfo
{
	vk::SurfaceKHR vulkanWindowSurface;
	WindowSize framebufferSize;
	uint32_t enabledLayerCount;
	char* const* enabledLayerNames;
	Model model;
};