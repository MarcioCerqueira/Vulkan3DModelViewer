#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandPool.h"
#include "ExceptionChecker.h"
#include "structs/CommandBufferBufferToBufferCopyInfo.h"
#include "structs/CommandBufferBufferToImageCopyInfo.h"
#include "structs/CommandBufferPipelineBarrierInfo.h"
#include "structs/CommandBufferRecordInfo.h"

class CommandBuffer
{
public:
	CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool, const int maxFramesInFlight);
	void copy(const CommandBufferBufferToBufferCopyInfo& commandBufferBufferToBufferCopyInfo);
	void copy(const CommandBufferBufferToImageCopyInfo& commandBufferBufferToImageCopyInfo);
	void pipelineBarrier(const CommandBufferPipelineBarrierInfo& commandBufferPipelineBarrierInfo);
	void record(const CommandBufferRecordInfo& commandBufferRecordInfo);
	void reset(const int frameIndex);

	const vk::CommandBuffer getVulkanCommandBuffer(const int frameIndex) const;

private:
	const vk::CommandBufferAllocateInfo buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool, uint32_t commandBufferCount);

	std::vector<vk::CommandBuffer> vulkanCommandBuffers;
};
