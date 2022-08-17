#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const vk::Device& vulkanLogicalDevice, const PhysicalDeviceProperties& physicalDeviceProperties, const vk::DeviceSize& contentSize) : buffer(vulkanLogicalDevice)
{
	createUniformData(physicalDeviceProperties, contentSize);
}

void UniformBuffer::createUniformData(const PhysicalDeviceProperties& physicalDeviceProperties, const vk::DeviceSize& contentSize)
{
	const vk::BufferUsageFlags uniformBufferUsage{ vk::BufferUsageFlagBits::eUniformBuffer };
	buffer.createVulkanBuffer(contentSize, uniformBufferUsage);
	const vk::MemoryPropertyFlags uniformMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	buffer.createVulkanBufferMemory(physicalDeviceProperties, uniformMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

void UniformBuffer::copyFromCPUToDeviceMemory(const ModelViewProjectionTransformation* data)
{
	buffer.copyFromCPUToDeviceMemory(data);
	size = sizeof(ModelViewProjectionTransformation);
}

const vk::Buffer UniformBuffer::getVulkanBuffer() const
{
	return buffer.getVulkanBuffer();
}

const size_t UniformBuffer::getSize() const
{
	return size;
}
