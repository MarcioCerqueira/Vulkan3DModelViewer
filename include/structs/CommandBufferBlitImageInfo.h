#pragma once

#include <vulkan/vulkan.hpp>

struct CommandBufferBlitImageInfo
{
	int frameIndex;
	const vk::Image& image;
	const vk::ImageLayout& srcLayout;
	const vk::ImageLayout& dstLayout;
	vk::ImageBlit blit;
};