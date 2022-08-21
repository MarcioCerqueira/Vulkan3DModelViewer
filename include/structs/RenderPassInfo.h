#pragma once

#include <vulkan/vulkan.hpp>

struct RenderPassInfo
{
	vk::Device vulkanLogicalDevice;
	vk::Format colorImageFormat;
	vk::Format depthImageFormat;
	vk::SampleCountFlagBits sampleCount;
};