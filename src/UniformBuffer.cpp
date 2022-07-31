#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize) : Buffer(vulkanLogicalDevice)
{
	createUniformData(vulkanPhysicalDevice, contentSize);
}

void UniformBuffer::createUniformData(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize)
{
	const vk::BufferUsageFlags uniformBufferUsage{ vk::BufferUsageFlagBits::eUniformBuffer };
	vulkanBuffer = Buffer::createVulkanBuffer(contentSize, uniformBufferUsage);
	const vk::MemoryPropertyFlags uniformMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	vulkanBufferMemory = Buffer::createVulkanBufferMemory(vulkanPhysicalDevice, uniformMemoryPropertyFlags);
	Buffer::bindBufferMemory(vulkanBuffer, vulkanBufferMemory);
}
