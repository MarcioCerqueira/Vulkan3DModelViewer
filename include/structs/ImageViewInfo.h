#pragma once

#include <vulkan/vulkan.hpp>

struct ImageViewInfo
{
	const vk::Device& vulkanLogicalDevice;
	const vk::Image& image;
	vk::Format format;
	vk::ImageAspectFlags aspectMask;
	uint32_t mipLevels;
};