#pragma once

#include <vulkan/vulkan.hpp>

class PhysicalDeviceProperties
{
public:
	virtual const uint32_t findMemoryType(uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags) const = 0;
	virtual const vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling& tiling, const vk::FormatFeatureFlags& features) const = 0;
	virtual const vk::PhysicalDevice getVulkanPhysicalDevice() const = 0;
protected:
	vk::PhysicalDevice vulkanPhysicalDevice;
};