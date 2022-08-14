#include "DescriptorPool.h"

DescriptorPool::DescriptorPool(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const std::vector<vk::DescriptorPoolSize> descriptorPoolSizes = {
		buildDescriptorPoolSize(vk::DescriptorType::eUniformBuffer, maxFramesInFlight),
		buildDescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, maxFramesInFlight) 
	};
	const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo{ buildDescriptorPoolCreateInfo(descriptorPoolSizes, maxFramesInFlight) };
	vulkanDescriptorPool = vulkanLogicalDevice.createDescriptorPool(descriptorPoolCreateInfo);
}

DescriptorPool::~DescriptorPool()
{
	vulkanLogicalDevice.destroyDescriptorPool(vulkanDescriptorPool);
}

const vk::DescriptorPoolSize DescriptorPool::buildDescriptorPoolSize(const vk::DescriptorType& descriptorType, const int maxFramesInFlight) const
{
	return vk::DescriptorPoolSize{
		.type = descriptorType,
		.descriptorCount = static_cast<uint32_t>(maxFramesInFlight)
	};
}

const vk::DescriptorPoolCreateInfo DescriptorPool::buildDescriptorPoolCreateInfo(const std::vector<vk::DescriptorPoolSize>& descriptorPoolSizes, const int maxFramesInFlight) const
{
	return vk::DescriptorPoolCreateInfo{
		.maxSets = static_cast<uint32_t>(maxFramesInFlight),
		.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size()),
		.pPoolSizes = descriptorPoolSizes.data()
	};
}

const vk::DescriptorPool DescriptorPool::getVulkanDescriptorPool() const
{
	return vulkanDescriptorPool;
}
