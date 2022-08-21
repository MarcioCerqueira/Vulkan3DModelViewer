#pragma once

#include <vulkan/vulkan.hpp>

struct CommandBufferBlitImageInfo
{
	int frameIndex;
	vk::Image image;
	vk::ImageLayout srcLayout;
	vk::ImageLayout dstLayout;
	vk::ImageBlit blit;
};