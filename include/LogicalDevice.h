#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>

class LogicalDevice
{
public:
	LogicalDevice(const vk::PhysicalDevice& vulkanPhysicalDevice, const std::optional<uint32_t>& graphicsFamilyIndex, const uint32_t enabledLayerCount, const char* const* enabledLayerNames);

private:
	vk::Device vulkanLogicalDevice;
};