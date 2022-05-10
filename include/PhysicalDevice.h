#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>

class PhysicalDevice
{
public:
	void pick(const std::vector<vk::PhysicalDevice>& physicalDevices);

private:
	void checkVulkanSupport(const std::vector<vk::PhysicalDevice>& physicalDevices) const;
	std::multimap<int, vk::PhysicalDevice> rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& physicalDevices) const noexcept;
	int rateSuitability(const vk::PhysicalDevice& vulkanPhysicalDevice) const noexcept;
	vk::PhysicalDevice selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const;

	vk::PhysicalDevice vulkanPhysicalDevice;
};