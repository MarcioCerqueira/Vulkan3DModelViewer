#include "StagingBuffer.h"

StagingBuffer::StagingBuffer(const vk::Device& vulkanLogicalDevice) : buffer(vulkanLogicalDevice), vulkanLogicalDevice(vulkanLogicalDevice)
{
}

void StagingBuffer::createStagingData(const vk::DeviceSize& contentSize, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags stagingBufferUsage{ vk::BufferUsageFlagBits::eTransferSrc };
	buffer.createVulkanBuffer(contentSize, stagingBufferUsage);
	const vk::MemoryPropertyFlags stagingMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	buffer.createVulkanBufferMemory(vulkanPhysicalDevice, stagingMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

void StagingBuffer::copyFromCPUToStagingMemory(const void* data)
{
	buffer.copyFromCPUToDeviceMemory(data);
}

void StagingBuffer::copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue, const vk::Buffer& dstBuffer)
{
	int commandBufferCount = 1;
	CommandBuffer commandBuffer(vulkanLogicalDevice, vulkanCommandPool, commandBufferCount);
	const CommandBufferCopyInfo commandBufferCopyInfo{
		.frameIndex = 0,
		.srcBuffer = buffer.getVulkanBuffer(),
		.dstBuffer = dstBuffer,
		.bufferSize = buffer.getBufferCreateInfo().size
	};
	commandBuffer.copy(commandBufferCopyInfo);
	graphicsQueue->submit(commandBuffer.getVulkanCommandBuffer(0));
	graphicsQueue->waitIdle();
}