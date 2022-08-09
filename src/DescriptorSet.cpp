#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(const DescriptorSetCreateInfo& descriptorSetCreateInfo) : vulkanLogicalDevice(descriptorSetCreateInfo.vulkanLogicalDevice)
{
	std::vector<vk::DescriptorSetLayout> vulkanDescriptorSetLayouts(descriptorSetCreateInfo.maxFramesInFlight, descriptorSetCreateInfo.vulkanDescriptorSetLayout);
	const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo{ createDescriptorSetAllocateInfo(descriptorSetCreateInfo, vulkanDescriptorSetLayouts) };
	vulkanDescriptorSets.resize(descriptorSetCreateInfo.maxFramesInFlight);
	vulkanDescriptorSets = vulkanLogicalDevice.allocateDescriptorSets(descriptorSetAllocateInfo);
}

const vk::DescriptorSetAllocateInfo DescriptorSet::createDescriptorSetAllocateInfo(const DescriptorSetCreateInfo& descriptorSetCreateInfo, std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts) const
{
	return vk::DescriptorSetAllocateInfo{
		.descriptorPool = descriptorSetCreateInfo.vulkanDescriptorPool,
		.descriptorSetCount = static_cast<uint32_t>(descriptorSetCreateInfo.maxFramesInFlight),
		.pSetLayouts = descriptorSetLayouts.data()
	};
}

void DescriptorSet::write(const vk::Buffer& vulkanUniformBuffer, int index)
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(index, vulkanDescriptorSets.size(), "Error in DescriptorSet! Index is out of bounds");
	const vk::DescriptorBufferInfo descriptorBufferInfo{ createBufferInfo(vulkanUniformBuffer) };
	const vk::WriteDescriptorSet writeDescriptorSet{ createWriteDescriptorSet(descriptorBufferInfo, index) };
	vulkanLogicalDevice.updateDescriptorSets(1, &writeDescriptorSet, 0, nullptr);
}

const vk::DescriptorBufferInfo DescriptorSet::createBufferInfo(const vk::Buffer& vulkanUniformBuffer) const
{
	return vk::DescriptorBufferInfo{
		.buffer = vulkanUniformBuffer,
		.offset = 0,
		.range = VK_WHOLE_SIZE
	};
}

const vk::WriteDescriptorSet DescriptorSet::createWriteDescriptorSet(const vk::DescriptorBufferInfo& descriptorBufferInfo, int frameIndex) const
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

const vk::DescriptorSet DescriptorSet::getVulkanDescriptorSet(int frameIndex) const
{
	ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(frameIndex, vulkanDescriptorSets.size(), "Error in DescriptorSet! Index is out of bounds");
	return vulkanDescriptorSets[frameIndex];
}