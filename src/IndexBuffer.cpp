#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const ContentBufferCreateInfo<uint32_t>& contentBufferCreateInfo) : buffer(contentBufferCreateInfo.vulkanLogicalDevice), stagingBuffer(contentBufferCreateInfo.vulkanLogicalDevice)
{
	indexCount = contentBufferCreateInfo.content.size();
	createIndexData(contentBufferCreateInfo.content, contentBufferCreateInfo.physicalDeviceProperties);
	const vk::DeviceSize contentSize{ sizeof(contentBufferCreateInfo.content[0]) * contentBufferCreateInfo.content.size() };
	stagingBuffer.createStagingData(contentSize, contentBufferCreateInfo.physicalDeviceProperties);
	stagingBuffer.copyFromCPUToStagingMemory(contentBufferCreateInfo.content.data());
	stagingBuffer.copyFromStagingToDeviceMemory(contentBufferCreateInfo.commandBuffers, buffer.getVulkanBuffer());
}

void IndexBuffer::createIndexData(const std::vector<uint32_t>& indices, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const vk::BufferUsageFlags indexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer };
	const vk::DeviceSize contentSize{ sizeof(indices[0]) * indices.size() };
	buffer.createVulkanBuffer(contentSize, indexBufferUsage);
	const vk::MemoryPropertyFlags indexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	buffer.createVulkanBufferMemory(physicalDeviceProperties, indexMemoryPropertyFlags);
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