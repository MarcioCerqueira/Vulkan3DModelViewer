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

void CommandBuffer::record(const vk::RenderPassBeginInfo& renderPassBeginInfo, const vk::Pipeline& graphicsPipeline, const int frameIndex)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const uint32_t vertexCount = 3;
	const uint32_t instanceCount = 1;
	const uint32_t firstVertex = 0;
	const uint32_t firstInstance = 0;
	vk::CommandBufferBeginInfo commandBufferBeginInfo;
	vulkanCommandBuffers[frameIndex].begin(commandBufferBeginInfo);
	vulkanCommandBuffers[frameIndex].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	vulkanCommandBuffers[frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	vulkanCommandBuffers[frameIndex].draw(vertexCount, instanceCount, firstVertex, firstInstance);
	vulkanCommandBuffers[frameIndex].endRenderPass();
	vulkanCommandBuffers[frameIndex].end();
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
