#include "StagingBuffer.h"

StagingBuffer::StagingBuffer(const vk::Device& vulkanLogicalDevice) : buffer(vulkanLogicalDevice), vulkanLogicalDevice(vulkanLogicalDevice)
{
}

void StagingBuffer::createStagingData(const vk::DeviceSize& contentSize, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const vk::BufferUsageFlags stagingBufferUsage{ vk::BufferUsageFlagBits::eTransferSrc };
	buffer.createVulkanBuffer(contentSize, stagingBufferUsage);
	const vk::MemoryPropertyFlags stagingMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	buffer.createVulkanBufferMemory(physicalDeviceProperties, stagingMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

void StagingBuffer::copyFromCPUToStagingMemory(const void* data)
{
	buffer.copyFromCPUToDeviceMemory(data);
}

void StagingBuffer::copyFromStagingToDeviceMemory(const std::shared_ptr<CommandBuffer>& commandBuffers, const vk::Buffer& dstBuffer)
{
	const CommandBufferBufferToBufferCopyInfo commandBufferBufferToBufferCopyInfo{
		.frameIndex = 0,
		.srcBuffer = buffer.getVulkanBuffer(),
		.dstBuffer = dstBuffer,
		.bufferSize = buffer.getBufferCreateInfo().size
	};
	commandBuffers->beginOneTimeSubmit(commandBufferBufferToBufferCopyInfo.frameIndex);
	commandBuffers->copy(commandBufferBufferToBufferCopyInfo);
	commandBuffers->endOneTimeSubmit(commandBufferBufferToBufferCopyInfo.frameIndex);
}

void StagingBuffer::copyFromStagingToDeviceMemory(const std::shared_ptr<CommandBuffer>& commandBuffers, const std::shared_ptr<Image>& dstImage)
{
	const CommandBufferBufferToImageCopyInfo commandBufferBufferToImageCopyInfo{
		.frameIndex = 0,
		.srcBuffer = buffer.getVulkanBuffer(),
		.dstImage = dstImage->getVulkanImage(),
		.bufferImageCopy = dstImage->buildBufferImageCopy()
	};
	commandBuffers->beginOneTimeSubmit(commandBufferBufferToImageCopyInfo.frameIndex);
	commandBuffers->copy(commandBufferBufferToImageCopyInfo);
	commandBuffers->endOneTimeSubmit(commandBufferBufferToImageCopyInfo.frameIndex);
}