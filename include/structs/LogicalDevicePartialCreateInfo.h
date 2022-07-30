#pragma once
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

#include "WindowSize.h"
#include "Vertex.h"

struct LogicalDevicePartialCreateInfo
{
	vk::SurfaceKHR vulkanWindowSurface;
	WindowSize framebufferSize;
	uint32_t enabledLayerCount;
	char* const* enabledLayerNames;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};