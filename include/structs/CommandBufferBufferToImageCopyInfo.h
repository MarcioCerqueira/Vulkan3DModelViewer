#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferBufferToImageCopyInfo
{
	int frameIndex;
	const vk::Buffer& srcBuffer;
	const vk::Image& dstImage; 
	vk::BufferImageCopy bufferImageCopy;
};