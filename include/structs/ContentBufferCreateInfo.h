#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandBuffer.h"

template<typename T>
struct ContentBufferCreateInfo
{
	vk::Device vulkanLogicalDevice;
	std::vector<T> content;
	vk::PhysicalDevice vulkanPhysicalDevice;
	std::shared_ptr<CommandBuffer> commandBuffers;
};