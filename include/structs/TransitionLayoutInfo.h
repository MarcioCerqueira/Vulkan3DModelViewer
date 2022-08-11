#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "GraphicsQueue.h"

struct TransitionLayoutInfo
{
	vk::ImageLayout oldLayout;
	vk::ImageLayout newLayout;
	vk::CommandPool vulkanCommandPool;
	std::shared_ptr<GraphicsQueue> graphicsQueue;
};