#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const ContentBufferCreateInfo<uint16_t>& contentBufferCreateInfo) : buffer(contentBufferCreateInfo.vulkanLogicalDevice), stagingBuffer(contentBufferCreateInfo.vulkanLogicalDevice)
{
	indexCount = contentBufferCreateInfo.content.size();
	createIndexData(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	const vk::DeviceSize contentSize{ sizeof(contentBufferCreateInfo.content[0]) * contentBufferCreateInfo.content.size() };
	stagingBuffer.createStagingData(contentSize, contentBufferCreateInfo.vulkanPhysicalDevice);
	stagingBuffer.copyFromCPUToStagingMemory(contentBufferCreateInfo.content.data());
	stagingBuffer.copyFromStagingToDeviceMemory(contentBufferCreateInfo.vulkanCommandPool, contentBufferCreateInfo.graphicsQueue, buffer.getVulkanBuffer());
}

void IndexBuffer::createIndexData(const std::vector<uint16_t>& indices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags indexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer };
	const vk::DeviceSize contentSize{ sizeof(indices[0]) * indices.size() };
	buffer.createVulkanBuffer(contentSize, indexBufferUsage);
	const vk::MemoryPropertyFlags indexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	buffer.createVulkanBufferMemory(vulkanPhysicalDevice, indexMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

const size_t IndexBuffer::getIndexCount() const
{
	return indexCount;
}

const vk::Buffer IndexBuffer::getVulkanBuffer() const
{
	return buffer.getVulkanBuffer();
}