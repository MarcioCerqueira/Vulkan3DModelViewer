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
	const int rate(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;

private:
	const int rateByPhysicalDeviceType(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	const int rateByQueueFamilyProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	const int rateByDeviceExtensionSupport(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
	const int rateBySwapChainProperties(const PhysicalDeviceSuitabilityRaterInfo& physicalDeviceSuitabilityRaterInfo) const;
};