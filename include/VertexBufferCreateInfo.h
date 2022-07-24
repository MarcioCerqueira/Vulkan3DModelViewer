#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "GraphicsQueue.h"

struct VertexBufferCreateInfo
{
	vk::Device vulkanLogicalDevice;
	std::vector<Vertex> vertices;
	vk::PhysicalDevice vulkanPhysicalDevice;
	vk::CommandPool vulkanCommandPool;
	std::shared_ptr<GraphicsQueue> graphicsQueue;
};