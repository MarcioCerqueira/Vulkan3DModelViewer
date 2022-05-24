#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <set>
#include <string>

#include "QueueFamilyIndices.h"
#include "SwapChain.h"

class PhysicalDeviceSuitabilityRater
{
public:
	int rate(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const std::vector<const char*> vulkanDeviceExtensions, const WindowSize& framebufferSize) const;

private:
	int rateByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	int rateByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateByDeviceExtensionSupport(const vk::PhysicalDevice& vulkanPhysicalDevice, const std::vector<const char*> vulkanDeviceExtensions) const;
	int rateBySwapChainProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize) const;

};