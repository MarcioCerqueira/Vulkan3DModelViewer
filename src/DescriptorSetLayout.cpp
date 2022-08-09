#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(const vk::Device& vulkanLogicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{ createDescriptorSetLayoutBinding() };
	const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{ buildDescriptorSetLayoutCreateInfo(descriptorSetLayoutBinding) };
	vulkanDescriptorSetLayout = vulkanLogicalDevice.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	vulkanLogicalDevice.destroyDescriptorSetLayout(vulkanDescriptorSetLayout);
}

const vk::DescriptorSetLayoutBinding DescriptorSetLayout::createDescriptorSetLayoutBinding() const
{
	return vk::DescriptorSetLayoutBinding{
		.binding = 0,
		.descriptorType = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eVertex
	};
}

const vk::DescriptorSetLayoutCreateInfo DescriptorSetLayout::buildDescriptorSetLayoutCreateInfo(const vk::DescriptorSetLayoutBinding& descriptorSetLayoutBinding) const
{
	return vk::DescriptorSetLayoutCreateInfo{
		.bindingCount = 1,
		.pBindings = &descriptorSetLayoutBinding
	};
}

const vk::DescriptorSetLayout DescriptorSetLayout::getVulkanDescriptorSetLayout() const
{
	return vulkanDescriptorSetLayout;
}