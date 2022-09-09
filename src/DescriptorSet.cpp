#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight) : vulkanLogicalDevice(vulkanLogicalDevice), descriptorPool(vulkanLogicalDevice, maxFramesInFlight), layout(vulkanLogicalDevice)
{
	std::vector<vk::DescriptorSetLayout> vulkanDescriptorSetLayouts(maxFramesInFlight, layout.getVulkanDescriptorSetLayout());
	const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo{ createDescriptorSetAllocateInfo(descriptorPool.getVulkanDescriptorPool(), maxFramesInFlight, vulkanDescriptorSetLayouts) };
	vulkanDescriptorSets.resize(maxFramesInFlight);
	vulkanDescriptorSets = vulkanLogicalDevice.allocateDescriptorSets(descriptorSetAllocateInfo);
}

vk::DescriptorSetAllocateInfo DescriptorSet::createDescriptorSetAllocateInfo(const vk::DescriptorPool& vulkanDescriptorPool, const int maxFramesInFlight, std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts) const
{
	return vk::DescriptorSetAllocateInfo{
		.descriptorPool = vulkanDescriptorPool,
		.descriptorSetCount = static_cast<uint32_t>(maxFramesInFlight),
		.pSetLayouts = descriptorSetLayouts.data()
	};
}

void DescriptorSet::write(const std::shared_ptr<UniformBuffer>& uniformBuffer, const std::shared_ptr<Image>& image, int index)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(index, vulkanDescriptorSets.size(), "Error in DescriptorSet! Index is out of bounds");
	const vk::DescriptorBufferInfo descriptorBufferInfo{ createBufferInfo(uniformBuffer->getVulkanBuffer(), uniformBuffer->getSize()) };
	const vk::DescriptorImageInfo descriptorImageInfo{ createImageInfo(image) };
	const std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
		createWriteDescriptorSet(descriptorBufferInfo, index),
		createWriteDescriptorSet(descriptorImageInfo, index)
	};
	vulkanLogicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

vk::DescriptorBufferInfo DescriptorSet::createBufferInfo(const vk::Buffer& vulkanUniformBuffer, const size_t uniformBufferObjectSize) const
{
	return vk::DescriptorBufferInfo{
		.buffer = vulkanUniformBuffer,
		.offset = 0,
		.range = uniformBufferObjectSize
	};
}

vk::WriteDescriptorSet DescriptorSet::createWriteDescriptorSet(const vk::DescriptorBufferInfo& descriptorBufferInfo, int frameIndex) const
{
	return vk::WriteDescriptorSet{
		.dstSet = vulkanDescriptorSets[frameIndex],
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = vk::DescriptorType::eUniformBuffer,
		.pBufferInfo = &descriptorBufferInfo
	};
}

vk::DescriptorImageInfo DescriptorSet::createImageInfo(const std::shared_ptr<Image>& image) const
{
	return vk::DescriptorImageInfo{
		.sampler = image->getVulkanSampler(),
		.imageView = image->getVulkanImageView(),
		.imageLayout = image->getImageLayout()
	};
}

vk::WriteDescriptorSet DescriptorSet::createWriteDescriptorSet(const vk::DescriptorImageInfo& descriptorImageInfo, int frameIndex) const
{
	return vk::WriteDescriptorSet{
		.dstSet = vulkanDescriptorSets[frameIndex],
		.dstBinding = 1,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = vk::DescriptorType::eCombinedImageSampler,
		.pImageInfo = &descriptorImageInfo
	};
}

const vk::DescriptorSet& DescriptorSet::getVulkanDescriptorSet(int frameIndex) const
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanDescriptorSets.size(), "Error in DescriptorSet! Index is out of bounds");
	return vulkanDescriptorSets[frameIndex];
}

const vk::DescriptorSetLayout& DescriptorSet::getVulkanDescriptorSetLayout() const
{
	return layout.getVulkanDescriptorSetLayout();
}