#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandPool.h"
#include "GraphicsQueue.h"
#include "ExceptionChecker.h"
#include "structs/CommandBufferBufferToBufferCopyInfo.h"
#include "structs/CommandBufferBufferToImageCopyInfo.h"
#include "structs/CommandBufferPipelineBarrierInfo.h"
#include "structs/CommandBufferRecordInfo.h"
#include "structs/CommandBufferBlitImageInfo.h"

class CommandBuffer
{
public:
	CommandBuffer(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& graphicsFamilyIndex, const int maxFramesInFlight);
	void beginOneTimeSubmit(const int frameIndex);
	void endOneTimeSubmit(const int frameIndex);
	void copy(const CommandBufferBufferToBufferCopyInfo& commandBufferBufferToBufferCopyInfo);
	void copy(const CommandBufferBufferToImageCopyInfo& commandBufferBufferToImageCopyInfo);
	void pipelineBarrier(const CommandBufferPipelineBarrierInfo& commandBufferPipelineBarrierInfo);
	void record(const CommandBufferRecordInfo& commandBufferRecordInfo);
	void blitImage(const CommandBufferBlitImageInfo& commandBuferBlitImageInfo);
	void reset(const int frameIndex);
	void submit(std::shared_ptr<SynchronizationObjects>& synchronizationObjects, const int frameIndex);
	
	const vk::CommandBuffer& getVulkanCommandBuffer(const int frameIndex) const;

private:
	vk::CommandBufferAllocateInfo buildCommandBufferAllocateInfo() const;
	vk::Viewport buildViewport(const vk::Extent2D& swapChainExtent) const;
	vk::Rect2D buildScissor(const vk::Extent2D& swapChainExtent) const;

	std::vector<vk::CommandBuffer> vulkanCommandBuffers;
	GraphicsQueue graphicsQueue;
	const CommandPool commandPool;
};
