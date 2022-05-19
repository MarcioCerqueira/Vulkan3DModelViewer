#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

class LogicalDevice
{
public:
	LogicalDevice(const std::optional<uint32_t>& graphicsFamilyIndex, const std::optional<uint32_t>& presentFamilyIndex);
	void create(const vk::PhysicalDevice& vulkanPhysicalDevice, const uint32_t enabledLayerCount, const char* const* enabledLayerNames);

private:
	vk::Device vulkanLogicalDevice; 
	const std::optional<uint32_t> graphicsFamilyIndex; 
	const std::optional<uint32_t> presentFamilyIndex;
};