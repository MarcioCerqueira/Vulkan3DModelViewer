#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const ContentBufferCreateInfo<Vertex>& contentBufferCreateInfo) : buffer(contentBufferCreateInfo.vulkanLogicalDevice), stagingBuffer(contentBufferCreateInfo.vulkanLogicalDevice)
{
	vertexCount = contentBufferCreateInfo.content.size();
	createVertexData(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	const vk::DeviceSize contentSize{ sizeof(contentBufferCreateInfo.content[0]) * contentBufferCreateInfo.content.size() };
	stagingBuffer.createStagingData(contentSize, contentBufferCreateInfo.vulkanPhysicalDevice);
	stagingBuffer.copyFromCPUToStagingMemory(contentBufferCreateInfo.content.data());
	stagingBuffer.copyFromStagingToDeviceMemory(contentBufferCreateInfo.vulkanCommandPool, contentBufferCreateInfo.graphicsQueue, buffer.getVulkanBuffer());
}

void VertexBuffer::createVertexData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags vertexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer };
	const vk::DeviceSize contentSize{ sizeof(vertices[0]) * vertices.size() };
	buffer.createVulkanBuffer(contentSize, vertexBufferUsage);
	const vk::MemoryPropertyFlags vertexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	buffer.createVulkanBufferMemory(vulkanPhysicalDevice, vertexMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

const size_t VertexBuffer::getVertexCount() const
{
	return vertexCount;
}

const vk::Buffer VertexBuffer::getVulkanBuffer() const
{
	return buffer.getVulkanBuffer();
}