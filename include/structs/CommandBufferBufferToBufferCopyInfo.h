#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferBufferToBufferCopyInfo
{
	int frameIndex;
	const vk::Buffer& srcBuffer;
	const vk::Buffer& dstBuffer;
	vk::DeviceSize bufferSize;
};