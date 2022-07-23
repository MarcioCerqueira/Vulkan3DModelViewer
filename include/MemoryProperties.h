#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class MemoryProperties
{
public:
	static uint32_t findMemoryType(const vk::PhysicalDevice& vulkanPhysicalDevice, uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags);
};