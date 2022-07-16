#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandPool.h"
#include "ExceptionChecker.h"

class CommandBuffer
{
public:
	CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool, const int maxFramesInFlight);

	const vk::CommandBuffer getVulkanCommandBuffer(const int frameIndex) const;

	void record(const vk::RenderPassBeginInfo& renderPassBeginInfo, const vk::Pipeline& graphicsPipeline, const int frameIndex);
	void reset(const int frameIndex);

private:
	const vk::CommandBufferAllocateInfo buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool, uint32_t commandBufferCount);

	std::vector<vk::CommandBuffer> vulkanCommandBuffers;
};
