#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const ContentBufferCreateInfo<uint16_t>& contentBufferCreateInfo) : Buffer(contentBufferCreateInfo.vulkanLogicalDevice)
{
	indexCount = contentBufferCreateInfo.content.size();
	createIndexData(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	Buffer::createStagingData<uint16_t>(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	Buffer::copyFromCPUToStagingMemory<uint16_t>(contentBufferCreateInfo.content);
	Buffer::copyFromStagingToDeviceMemory(contentBufferCreateInfo.vulkanCommandPool, contentBufferCreateInfo.graphicsQueue);
}

void IndexBuffer::createIndexData(const std::vector<uint16_t>& indices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags indexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer };
	vulkanBuffer = Buffer::createVulkanBuffer<uint16_t>(indices, indexBufferUsage);
	const vk::MemoryPropertyFlags vertexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	vulkanBufferMemory = Buffer::createVulkanBufferMemory(vulkanPhysicalDevice, vertexMemoryPropertyFlags);
	Buffer::bindBufferMemory(vulkanBuffer, vulkanBufferMemory);
}

const size_t IndexBuffer::getIndexCount() const
{
	return indexCount;
}