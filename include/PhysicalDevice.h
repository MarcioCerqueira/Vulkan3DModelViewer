#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <optional>
#include <memory>

#include "LogicalDevice.h"

class PhysicalDevice
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface);
	std::unique_ptr<LogicalDevice> createLogicalDevice(uint32_t enabledLayerCount, char* const* enabledLayerNames, const vk::SurfaceKHR& vulkanWindowSurface);

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const;
	std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateSuitabilityByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	int rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	QueueFamilyIndicesModel computeQueueFamilyIndices(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateSuitabilityByDeviceExtensionSupport(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	std::optional<uint32_t> retrieveValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	std::optional<uint32_t> retrieveValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	vk::PhysicalDevice vulkanPhysicalDevice;
	const std::vector<const char*> vulkanDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};