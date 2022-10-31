#include "PhysicalDevice.h"

void PhysicalDevice::pick(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface)
{
	checkVulkanSupport(vulkanPhysicalDevices);
	std::multimap<int, vk::PhysicalDevice> mostSuitableDevices{ rateMostSuitablePhysicalDevices(vulkanPhysicalDevices, vulkanWindowSurface) };
	vulkanPhysicalDevice = selectMostSuitablePhysicalDevice(mostSuitableDevices);
	std::cout << "Selected physical device: " << vulkanPhysicalDevice.getProperties().deviceName << std::endl;
}

void PhysicalDevice::checkVulkanSupport(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices) const
{
	if (vulkanPhysicalDevices.size() == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
}

std::multimap<int, vk::PhysicalDevice> PhysicalDevice::rateMostSuitablePhysicalDevices(const std::vector<vk::PhysicalDevice>& vulkanPhysicalDevices, const vk::SurfaceKHR& vulkanWindowSurface) const
{
	std::multimap<int, vk::PhysicalDevice> mostSuitablePhysicalDevices;
	std::ranges::for_each(vulkanPhysicalDevices, [this, vulkanWindowSurface, &mostSuitablePhysicalDevices](auto& vulkanPhysicalDevice) {
		PhysicalDeviceSuitabilityRaterInfo physicalDeviceSuitabilityRaterInfo{
			.vulkanPhysicalDevice = vulkanPhysicalDevice,
			.vulkanWindowSurface = vulkanWindowSurface,
			.vulkanDeviceExtensions = vulkanDeviceExtensions
		};
		int score{ physicalDeviceSuitabilityRater.rate(physicalDeviceSuitabilityRaterInfo) };
		mostSuitablePhysicalDevices.insert(std::make_pair(score, vulkanPhysicalDevice));
	});
	return mostSuitablePhysicalDevices;
}

vk::PhysicalDevice PhysicalDevice::selectMostSuitablePhysicalDevice(const std::multimap<int, vk::PhysicalDevice>& mostSuitablePhysicalDevices) const
{
	if (mostSuitablePhysicalDevices.rbegin()->first > 0)
	{
		return mostSuitablePhysicalDevices.rbegin()->second;
	}
	else
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

std::unique_ptr<LogicalDevice> PhysicalDevice::createLogicalDevice(const LogicalDevicePartialCreateInfo& logicalDevicePartialCreateInfo)
{
	const LogicalDeviceCreateInfo logicalDeviceCreateInfo{
		.queueFamilyIndices = QueueFamilyIndices(vulkanPhysicalDevice, logicalDevicePartialCreateInfo.vulkanWindowSurface),
		.physicalDeviceProperties = *this,
		.vulkanWindowSurface = logicalDevicePartialCreateInfo.vulkanWindowSurface,
		.vulkanDeviceExtensions = vulkanDeviceExtensions,
		.framebufferSize = logicalDevicePartialCreateInfo.framebufferSize,
		.enabledLayerCount = logicalDevicePartialCreateInfo.enabledLayerCount,
		.enabledLayerNames = logicalDevicePartialCreateInfo.enabledLayerNames,
		.model = logicalDevicePartialCreateInfo.model
	};
	return std::make_unique<LogicalDevice>(logicalDeviceCreateInfo);
}

uint32_t PhysicalDevice::findMemoryType(uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	std::vector<uint32_t> indices(vulkanPhysicalDevice.getMemoryProperties().memoryTypeCount);
	std::ranges::generate(indices, [index = 0]() mutable { return index++; });
	auto memoryType = std::ranges::find_if(indices, [this, memoryTypeFilter, &memoryPropertyFlags](const uint32_t index) {
		bool firstCondition = memoryTypeFilter & (1 << index);
		bool secondCondition = (vulkanPhysicalDevice.getMemoryProperties().memoryTypes[index].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags;
		return firstCondition && secondCondition;
	});
	if (memoryType != indices.end())
	{
		return *memoryType;
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

vk::Format PhysicalDevice::findSupportedFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling& tiling, const vk::FormatFeatureFlags& features) const
{
	auto supportedFormat = std::ranges::find_if(candidates, [this, &tiling, &features](const auto& format) {
		const vk::FormatProperties formatProperties{ vulkanPhysicalDevice.getFormatProperties(format) };
		bool isLinear = (tiling == vk::ImageTiling::eLinear && (formatProperties.linearTilingFeatures & features) == features);
		bool isOptimal = (tiling == vk::ImageTiling::eOptimal && (formatProperties.optimalTilingFeatures & features) == features);
		return isLinear || isOptimal;
	});
	if (supportedFormat != candidates.end())
	{
		return *supportedFormat;
	}
	throw std::runtime_error("Failed to find supported features!");
}

vk::SampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount() const
{
	const vk::PhysicalDeviceProperties vulkanPhysicalDeviceProperties{ vulkanPhysicalDevice.getProperties() };
	const vk::SampleCountFlags sampleCounts{ vulkanPhysicalDeviceProperties.limits.framebufferColorSampleCounts & vulkanPhysicalDeviceProperties.limits.framebufferDepthSampleCounts };
	if (sampleCounts & vk::SampleCountFlagBits::e64)
	{
		return vk::SampleCountFlagBits::e64;
	}
	else if (sampleCounts & vk::SampleCountFlagBits::e32)
	{
		return vk::SampleCountFlagBits::e32;
	}
	else if (sampleCounts & vk::SampleCountFlagBits::e16)
	{
		return vk::SampleCountFlagBits::e16;
	}
	else if (sampleCounts & vk::SampleCountFlagBits::e8)
	{
		return vk::SampleCountFlagBits::e8;
	}
	else if (sampleCounts & vk::SampleCountFlagBits::e4)
	{
		return vk::SampleCountFlagBits::e4;
	}
	else if (sampleCounts & vk::SampleCountFlagBits::e2)
	{
		return vk::SampleCountFlagBits::e2;
	}
	else
	{
		return vk::SampleCountFlagBits::e1;
	}
}

const vk::PhysicalDevice& PhysicalDevice::getVulkanPhysicalDevice() const
{
	return vulkanPhysicalDevice;
}
