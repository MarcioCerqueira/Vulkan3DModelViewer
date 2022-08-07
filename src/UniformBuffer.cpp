#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize) : buffer(vulkanLogicalDevice)
{
	createUniformData(vulkanPhysicalDevice, contentSize);
}

void UniformBuffer::createUniformData(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize)
{
	const vk::BufferUsageFlags uniformBufferUsage{ vk::BufferUsageFlagBits::eUniformBuffer };
	buffer.createVulkanBuffer(contentSize, uniformBufferUsage);
	const vk::MemoryPropertyFlags uniformMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	buffer.createVulkanBufferMemory(vulkanPhysicalDevice, uniformMemoryPropertyFlags);
	buffer.bindBufferMemory();
}

void UniformBuffer::copyFromCPUToDeviceMemory(const ModelViewProjectionTransformation* data)
{
	buffer.copyFromCPUToDeviceMemory(data);
}

const vk::Buffer UniformBuffer::getVulkanBuffer() const
{
	return buffer.getVulkanBuffer();
}
