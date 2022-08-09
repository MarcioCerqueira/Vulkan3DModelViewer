#include "DescriptorPool.h"

DescriptorPool::DescriptorPool(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::DescriptorPoolSize descriptorPoolSize{ buildDescriptorPoolSize(maxFramesInFlight) };
	const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo{ buildDescriptorPoolCreateInfo(descriptorPoolSize, maxFramesInFlight) };
	vulkanDescriptorPool = vulkanLogicalDevice.createDescriptorPool(descriptorPoolCreateInfo);
}

DescriptorPool::~DescriptorPool()
{
	vulkanLogicalDevice.destroyDescriptorPool(vulkanDescriptorPool);
}

const vk::DescriptorPoolSize DescriptorPool::buildDescriptorPoolSize(const int maxFramesInFlight) const
{
	return vk::DescriptorPoolSize{
		.type = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = static_cast<uint32_t>(maxFramesInFlight)
	};
}

const vk::DescriptorPoolCreateInfo DescriptorPool::buildDescriptorPoolCreateInfo(const vk::DescriptorPoolSize& descriptorPoolSize, const int maxFramesInFlight) const
{
	return vk::DescriptorPoolCreateInfo{
		.maxSets = static_cast<uint32_t>(maxFramesInFlight),
		.poolSizeCount = 1,
		.pPoolSizes = &descriptorPoolSize
	};
}

const vk::DescriptorPool DescriptorPool::getVulkanDescriptorPool() const
{
	return vulkanDescriptorPool;
}
