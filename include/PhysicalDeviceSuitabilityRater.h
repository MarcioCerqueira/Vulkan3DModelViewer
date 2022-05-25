#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <set>
#include <string>

#include "PhysicalDeviceSuitabilityRaterInfo.h"
#include "QueueFamilyIndices.h"
#include "SwapChain.h"

class PhysicalDeviceSuitabilityRater
{
public:
	int rate(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;

private:
	int rateByPhysicalDeviceType(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateByQueueFamilyProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateByDeviceExtensionSupport(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	int rateBySwapChainProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
};