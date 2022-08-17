#pragma once

#include <vulkan/vulkan.hpp>

struct ImageViewInfo
{
	vk::Device vulkanLogicalDevice;
	vk::Image image;
	vk::Format format;
	vk::ImageAspectFlags aspectMask;
};