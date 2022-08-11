#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferPipelineBarrierInfo
{
	int frameIndex;
	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
	vk::ImageMemoryBarrier imageMemoryBarrier;
};
