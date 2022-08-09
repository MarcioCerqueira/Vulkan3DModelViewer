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
