#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandBuffer.h"
#include "PhysicalDeviceProperties.h"

template<typename T>
struct ContentBufferCreateInfo
{
	const vk::Device& vulkanLogicalDevice;
	std::vector<T> content;
	const PhysicalDeviceProperties& physicalDeviceProperties;
	std::shared_ptr<CommandBuffer> commandBuffers;
};