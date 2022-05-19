#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>
#include <optional>

#include "LogicalDevice.h"

class PhysicalDevice
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface);
	std::unique_ptr<LogicalDevice> createLogicalDevice(const uint32_t enabledLayerCount, const char* const* enabledLayerNames, const vk::SurfaceKHR& vulkanWindowSurface);

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const;
	std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	int rateSuitabilityByPhysicalDeviceType(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	int rateSuitabilityByQueueFamilyProperties(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	std::optional<uint32_t> retrieveValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	std::optional<uint32_t> retrieveValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface) const;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	vk::PhysicalDevice vulkanPhysicalDevice;
};