#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferBufferToImageCopyInfo
{
	int frameIndex;
	vk::Buffer srcBuffer;
	vk::Image dstImage; 
	vk::BufferImageCopy bufferImageCopy;
};