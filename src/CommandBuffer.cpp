#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(const vk::Device& vulkanLogicalDevice, const vk::CommandPool& vulkanCommandPool)
{
	vk::CommandBufferAllocateInfo commandBufferAllocateInfo{ buildCommandBufferAllocateInfo(vulkanCommandPool) };
	vulkanCommandBuffers = vulkanLogicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);
}

vk::CommandBufferAllocateInfo CommandBuffer::buildCommandBufferAllocateInfo(const vk::CommandPool& vulkanCommandPool)
{
	return vk::CommandBufferAllocateInfo{
		.commandPool = vulkanCommandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1
	};
}

void CommandBuffer::record(const vk::RenderPassBeginInfo& renderPassBeginInfo, const vk::Pipeline& graphicsPipeline)
{
	vk::CommandBufferBeginInfo commandBufferBeginInfo;
	vulkanCommandBuffers[0].begin(commandBufferBeginInfo);
	vulkanCommandBuffers[0].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	vulkanCommandBuffers[0].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

	uint32_t vertexCount = 3;
	uint32_t instanceCount = 1;
	uint32_t firstVertex = 0;
	uint32_t firstInstance = 0;
	vulkanCommandBuffers[0].draw(vertexCount, instanceCount, firstVertex, firstInstance);
	vulkanCommandBuffers[0].endRenderPass();
	vulkanCommandBuffers[0].end();
}
