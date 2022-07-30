#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "GraphicsQueue.h"

template<typename T>
struct ContentBufferCreateInfo
{
	vk::Device vulkanLogicalDevice;
	std::vector<T> content;
	vk::PhysicalDevice vulkanPhysicalDevice;
	vk::CommandPool vulkanCommandPool;
	std::shared_ptr<GraphicsQueue> graphicsQueue;
};