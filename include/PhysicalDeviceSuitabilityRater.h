#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <set>
#include <string>

#include "structs/PhysicalDeviceSuitabilityRaterInfo.h"
#include "QueueFamilyIndices.h"
#include "SwapChain.h"
#include "LogicalDevice.h"
#include "ValidationLayer.h"

class PhysicalDeviceSuitabilityRater
{
public:
	int rate(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;

private:
	int rateByPhysicalDeviceType(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateByQueueFamilyProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateByDeviceExtensionSupport(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateBySwapChainProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateByDeviceSupportedFeatures(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	const std::unordered_map<vk::PhysicalDeviceType, int> physicalDeviceTypeRater = {
		{vk::PhysicalDeviceType::eDiscreteGpu, 1000},
		{vk::PhysicalDeviceType::eIntegratedGpu, 500},
		{vk::PhysicalDeviceType::eCpu, 100}
	};
};