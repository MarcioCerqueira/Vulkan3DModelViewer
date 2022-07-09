#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <memory>

#include "LogicalDevice.h"
#include "SwapChain.h"
#include "PhysicalDeviceSuitabilityRater.h"
#include "ValidationLayer.h"

class PhysicalDevice
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface);
	std::unique_ptr<LogicalDevice> createLogicalDevice(const ValidationLayer& validationLayer, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize);

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const;
	const std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	vk::PhysicalDevice vulkanPhysicalDevice;
	const std::vector<const char*> vulkanDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const PhysicalDeviceSuitabilityRater physicalDeviceSuitabilityRater;
};