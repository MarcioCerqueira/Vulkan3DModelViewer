#pragma once

#include <vulkan/vulkan.hpp>

struct RenderPassInfo
{
	const vk::Device& vulkanLogicalDevice;
	vk::Format colorImageFormat;
	vk::Format depthImageFormat;
	vk::SampleCountFlagBits sampleCount;
};