#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <memory>

#include "LogicalDevice.h"
#include "structs/LogicalDevicePartialCreateInfo.h"
#include "SwapChain.h"
#include "PhysicalDeviceSuitabilityRater.h"
#include "PhysicalDeviceProperties.h"
#include "ValidationLayer.h"

class PhysicalDevice : public PhysicalDeviceProperties
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface);
	std::unique_ptr<LogicalDevice> createLogicalDevice(const LogicalDevicePartialCreateInfo& logicalDevicePartialCreateInfo);
	const uint32_t findMemoryType(uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags) const override;
	const vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling& tiling, const vk::FormatFeatureFlags& features) const override;
	const vk::PhysicalDevice getVulkanPhysicalDevice() const override;

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const;
	const std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	const std::vector<const char*> vulkanDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const PhysicalDeviceSuitabilityRater physicalDeviceSuitabilityRater;
};