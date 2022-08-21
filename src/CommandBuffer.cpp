#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& graphicsFamilyIndex, const int maxFramesInFlight) : commandPool(vulkanLogicalDevice, graphicsFamilyIndex), graphicsQueue(vulkanLogicalDevice, graphicsFamilyIndex)
{
	vulkanCommandBuffers.resize(maxFramesInFlight);
	const vk::CommandBufferAllocateInfo commandBufferAllocateInfo{ buildCommandBufferAllocateInfo() };
	vulkanCommandBuffers = vulkanLogicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);
}

const vk::CommandBufferAllocateInfo CommandBuffer::buildCommandBufferAllocateInfo() const
{
	return vk::CommandBufferAllocateInfo{
		.commandPool = commandPool.getVulkanCommandPool(),
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = static_cast<uint32_t>(vulkanCommandBuffers.size())
	};
}

void CommandBuffer::beginOneTimeSubmit(const int frameIndex)
{
	const vk::CommandBufferBeginInfo commandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
	vulkanCommandBuffers[frameIndex].begin(commandBufferBeginInfo);
}

void CommandBuffer::endOneTimeSubmit(const int frameIndex)
{
	vulkanCommandBuffers[frameIndex].end();
	graphicsQueue.submit(vulkanCommandBuffers[frameIndex]);
	graphicsQueue.waitIdle();
}

void CommandBuffer::copy(const CommandBufferBufferToBufferCopyInfo& commandBufferBufferToBufferCopyInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferBufferToBufferCopyInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const vk::BufferCopy bufferCopyRegion{ .size = commandBufferBufferToBufferCopyInfo.bufferSize };
	vulkanCommandBuffers[commandBufferBufferToBufferCopyInfo.frameIndex].copyBuffer(commandBufferBufferToBufferCopyInfo.srcBuffer, commandBufferBufferToBufferCopyInfo.dstBuffer, 1, &bufferCopyRegion);
}

void CommandBuffer::copy(const CommandBufferBufferToImageCopyInfo& commandBufferBufferToImageCopyInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferBufferToImageCopyInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const uint32_t regionCount{ 1 };
	vulkanCommandBuffers[commandBufferBufferToImageCopyInfo.frameIndex].copyBufferToImage(commandBufferBufferToImageCopyInfo.srcBuffer, commandBufferBufferToImageCopyInfo.dstImage, vk::ImageLayout::eTransferDstOptimal, regionCount, &commandBufferBufferToImageCopyInfo.bufferImageCopy);
}

void CommandBuffer::pipelineBarrier(const CommandBufferPipelineBarrierInfo& commandBufferPipelineBarrierInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferPipelineBarrierInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const vk::DependencyFlags dependencyFlags;
	const uint32_t memoryBarrierCount = 0;
	const vk::MemoryBarrier* memoryBarrier = nullptr;
	const uint32_t bufferMemoryBarrierCount = 0;
	const vk::BufferMemoryBarrier* bufferMemoryBarrier = nullptr;
	const uint32_t imageMemoryBarrierCount = 1;
	vulkanCommandBuffers[commandBufferPipelineBarrierInfo.frameIndex].pipelineBarrier(commandBufferPipelineBarrierInfo.srcStage, commandBufferPipelineBarrierInfo.dstStage, dependencyFlags, memoryBarrierCount, memoryBarrier, bufferMemoryBarrierCount, bufferMemoryBarrier, imageMemoryBarrierCount, &commandBufferPipelineBarrierInfo.imageMemoryBarrier);
}

void CommandBuffer::record(const CommandBufferRecordInfo& commandBufferRecordInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferRecordInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	vk::Buffer vulkanVertexBuffers[] = { commandBufferRecordInfo.vulkanVertexBuffer };
	vk::DeviceSize offsets[] = { commandBufferRecordInfo.offset };
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].begin(vk::CommandBufferBeginInfo{});
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].beginRenderPass(commandBufferRecordInfo.renderPassBeginInfo, vk::SubpassContents::eInline);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, commandBufferRecordInfo.graphicsPipeline);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindVertexBuffers(commandBufferRecordInfo.firstBinding, commandBufferRecordInfo.bindingCount, vulkanVertexBuffers, offsets);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindIndexBuffer(commandBufferRecordInfo.vulkanIndexBuffer, commandBufferRecordInfo.offset, commandBufferRecordInfo.indexType);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, commandBufferRecordInfo.vulkanPipelineLayout, commandBufferRecordInfo.firstDescriptorSet, commandBufferRecordInfo.descriptorSetCount, &commandBufferRecordInfo.vulkanDescriptorSet, commandBufferRecordInfo.descriptorDynamicOffsetCount, commandBufferRecordInfo.descriptorDynamicOffset);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].drawIndexed(static_cast<uint32_t>(commandBufferRecordInfo.indexCount), commandBufferRecordInfo.instanceCount, commandBufferRecordInfo.firstIndex, commandBufferRecordInfo.vertexOffset, commandBufferRecordInfo.firstInstance);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].endRenderPass();
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].end();
}

void CommandBuffer::blitImage(const CommandBufferBlitImageInfo& commandBuferBlitImageInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBuferBlitImageInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const uint32_t regionCount{ 1 };
	vulkanCommandBuffers[commandBuferBlitImageInfo.frameIndex].blitImage(commandBuferBlitImageInfo.image, commandBuferBlitImageInfo.srcLayout, commandBuferBlitImageInfo.image, commandBuferBlitImageInfo.dstLayout, regionCount, &commandBuferBlitImageInfo.blit, vk::Filter::eLinear);
}

void CommandBuffer::reset(const int frameIndex)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	vulkanCommandBuffers[frameIndex].reset();
}


void CommandBuffer::submit(std::shared_ptr<SynchronizationObjects>& synchronizationObjects, const int frameIndex)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	graphicsQueue.submit(synchronizationObjects, vulkanCommandBuffers[frameIndex]);
}

const vk::CommandBuffer CommandBuffer::getVulkanCommandBuffer(const int frameIndex) const
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	return vulkanCommandBuffers[frameIndex];
}
