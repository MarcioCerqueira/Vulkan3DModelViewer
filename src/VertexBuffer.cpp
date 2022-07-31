#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const ContentBufferCreateInfo<Vertex>& contentBufferCreateInfo) : Buffer(contentBufferCreateInfo.vulkanLogicalDevice)
{
	vertexCount = contentBufferCreateInfo.content.size();
	createVertexData(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	Buffer::createStagingData<Vertex>(contentBufferCreateInfo.content, contentBufferCreateInfo.vulkanPhysicalDevice);
	Buffer::copyFromCPUToStagingMemory<Vertex>(contentBufferCreateInfo.content);
	Buffer::copyFromStagingToDeviceMemory(contentBufferCreateInfo.vulkanCommandPool, contentBufferCreateInfo.graphicsQueue);
}

void VertexBuffer::createVertexData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags vertexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer };
	const vk::DeviceSize contentSize{ sizeof(vertices[0]) * vertices.size() };
	vulkanBuffer = Buffer::createVulkanBuffer(contentSize, vertexBufferUsage);
	const vk::MemoryPropertyFlags vertexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	vulkanBufferMemory = Buffer::createVulkanBufferMemory(vulkanPhysicalDevice, vertexMemoryPropertyFlags);
	Buffer::bindBufferMemory(vulkanBuffer, vulkanBufferMemory);
}

const size_t VertexBuffer::getVertexCount() const
{
	return vertexCount;
}