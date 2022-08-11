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
	const int commandBufferCount{ 1 };
	CommandBuffer commandBuffer(vulkanLogicalDevice, vulkanCommandPool, commandBufferCount);
	const CommandBufferBufferToBufferCopyInfo commandBufferBufferToBufferCopyInfo{
		.frameIndex = 0,
		.srcBuffer = buffer.getVulkanBuffer(),
		.dstBuffer = dstBuffer,
		.bufferSize = buffer.getBufferCreateInfo().size
	};
	commandBuffer.copy(commandBufferBufferToBufferCopyInfo);
	graphicsQueue->submit(commandBuffer.getVulkanCommandBuffer(0));
	graphicsQueue->waitIdle();
}

void StagingBuffer::copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue, const std::shared_ptr<Image>& dstImage)
{
	const int commandBufferCount{ 1 };
	CommandBuffer commandBuffer(vulkanLogicalDevice, vulkanCommandPool, commandBufferCount);
	const CommandBufferBufferToImageCopyInfo commandBufferBufferToImageCopyInfo{
		.frameIndex = 0,
		.srcBuffer = buffer.getVulkanBuffer(),
		.dstImage = dstImage->getVulkanImage(),
		.bufferImageCopy = dstImage->buildBufferImageCopy()
	};
	commandBuffer.copy(commandBufferBufferToImageCopyInfo);
	graphicsQueue->submit(commandBuffer.getVulkanCommandBuffer(0));
	graphicsQueue->waitIdle();
}