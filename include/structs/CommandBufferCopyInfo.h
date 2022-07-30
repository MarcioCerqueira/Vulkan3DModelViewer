#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferCopyInfo
{
	int frameIndex;
	vk::Buffer srcBuffer;
	vk::Buffer dstBuffer;
	vk::DeviceSize bufferSize;
};