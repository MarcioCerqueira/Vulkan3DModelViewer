#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool, const int maxFramesInFlight)
{
	vulkanCommandBuffers.resize(maxFramesInFlight);
	const vk::CommandBufferAllocateInfo commandBufferAllocateInfo{ buildCommandBufferAllocateInfo(vulkanCommandPool, static_cast<uint32_t>(vulkanCommandBuffers.size())) };
	vulkanCommandBuffers = vulkanLogicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);
}

const vk::CommandBufferAllocateInfo CommandBuffer::buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool, uint32_t commandBufferCount)
{
	return vk::CommandBufferAllocateInfo{
		.commandPool = vulkanCommandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = commandBufferCount
	};
}

void CommandBuffer::copy(const CommandBufferCopyInfo& commandBufferCopyInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferCopyInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const vk::CommandBufferBeginInfo commandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
	const vk::BufferCopy bufferCopyRegion{ .size = commandBufferCopyInfo.bufferSize };
	vulkanCommandBuffers[commandBufferCopyInfo.frameIndex].begin(commandBufferBeginInfo);
	vulkanCommandBuffers[commandBufferCopyInfo.frameIndex].copyBuffer(commandBufferCopyInfo.srcBuffer, commandBufferCopyInfo.dstBuffer, 1, &bufferCopyRegion);
	vulkanCommandBuffers[commandBufferCopyInfo.frameIndex].end();
}

void CommandBuffer::record(const CommandBufferRecordInfo& commandBufferRecordInfo)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(commandBufferRecordInfo.frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const uint32_t firstBinding = 0;
	const uint32_t bindingCount = 1;
	const uint32_t instanceCount = 1;
	const uint32_t firstVertex = 0;
	const uint32_t firstInstance = 0;
	vk::Buffer vulkanVertexBuffers[] = { commandBufferRecordInfo.vulkanVertexBuffer };
	vk::DeviceSize offsets[] = { 0 };
	vk::CommandBufferBeginInfo commandBufferBeginInfo;
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].begin(commandBufferBeginInfo);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].beginRenderPass(commandBufferRecordInfo.renderPassBeginInfo, vk::SubpassContents::eInline);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, commandBufferRecordInfo.graphicsPipeline);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].bindVertexBuffers(firstBinding, bindingCount, vulkanVertexBuffers, offsets);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].draw(static_cast<uint32_t>(commandBufferRecordInfo.vertexCount), instanceCount, firstVertex, firstInstance);
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].endRenderPass();
	vulkanCommandBuffers[commandBufferRecordInfo.frameIndex].end();
}

void CommandBuffer::reset(const int frameIndex)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	vulkanCommandBuffers[frameIndex].reset();
}

const vk::CommandBuffer CommandBuffer::getVulkanCommandBuffer(const int frameIndex) const
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	return vulkanCommandBuffers[frameIndex];
}
