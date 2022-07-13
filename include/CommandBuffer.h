#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandPool.h"

class CommandBuffer
{
public:
	CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool);

	const vk::CommandBuffer getVulkanCommandBuffer(const int index) const;

	void record(const vk::RenderPassBeginInfo& renderPassBeginInfo, const vk::Pipeline& graphicsPipeline, const int imageIndex);
	void reset();

private:
	const vk::CommandBufferAllocateInfo buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool);

	std::vector<vk::CommandBuffer> vulkanCommandBuffers;
};
