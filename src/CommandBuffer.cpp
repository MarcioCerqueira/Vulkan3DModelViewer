#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool)
{
	const vk::CommandBufferAllocateInfo commandBufferAllocateInfo{ buildCommandBufferAllocateInfo(vulkanCommandPool) };
	vulkanCommandBuffers = vulkanLogicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);
}

const vk::CommandBufferAllocateInfo CommandBuffer::buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool)
{
	return vk::CommandBufferAllocateInfo{
		.commandPool = vulkanCommandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1
	};
}

void CommandBuffer::record(const vk::RenderPassBeginInfo& renderPassBeginInfo, const vk::Pipeline& graphicsPipeline, const int imageIndex)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(imageIndex, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	const uint32_t vertexCount = 3;
	const uint32_t instanceCount = 1;
	const uint32_t firstVertex = 0;
	const uint32_t firstInstance = 0;
	vk::CommandBufferBeginInfo commandBufferBeginInfo;
	vulkanCommandBuffers[imageIndex].begin(commandBufferBeginInfo);
	vulkanCommandBuffers[imageIndex].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	vulkanCommandBuffers[imageIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	vulkanCommandBuffers[imageIndex].draw(vertexCount, instanceCount, firstVertex, firstInstance);
	vulkanCommandBuffers[imageIndex].endRenderPass();
	vulkanCommandBuffers[imageIndex].end();
}

void CommandBuffer::reset()
{
	for (auto& vulkanCommandBuffer : vulkanCommandBuffers)
	{
		vulkanCommandBuffer.reset();
	}
}

const vk::CommandBuffer CommandBuffer::getVulkanCommandBuffer(const int index) const
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(index, vulkanCommandBuffers.size(), "Error in CommandBuffer! Index is out of bounds");
	return vulkanCommandBuffers[index];
}
