#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(const vk::Device& vulkanLogicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::DescriptorSetLayoutBinding uniformBufferDescriptorSetLayoutBinding{ createUniformBufferDescriptorSetLayoutBinding() };
	const vk::DescriptorSetLayoutBinding samplerDescriptorSetLayoutBinding{ createSamplerDescriptorSetLayoutBinding() };
	const std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = { uniformBufferDescriptorSetLayoutBinding, samplerDescriptorSetLayoutBinding };
	const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{ buildDescriptorSetLayoutCreateInfo(descriptorSetLayoutBindings) };
	vulkanDescriptorSetLayout = vulkanLogicalDevice.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	vulkanLogicalDevice.destroyDescriptorSetLayout(vulkanDescriptorSetLayout);
}

vk::DescriptorSetLayoutBinding DescriptorSetLayout::createUniformBufferDescriptorSetLayoutBinding() const
{
	return vk::DescriptorSetLayoutBinding{
		.binding = 0,
		.descriptorType = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eVertex
	};
}

vk::DescriptorSetLayoutBinding DescriptorSetLayout::createSamplerDescriptorSetLayoutBinding() const
{
	return vk::DescriptorSetLayoutBinding{
		.binding = 1,
		.descriptorType = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eFragment
	};
}

vk::DescriptorSetLayoutCreateInfo DescriptorSetLayout::buildDescriptorSetLayoutCreateInfo(const std::vector<vk::DescriptorSetLayoutBinding>& descriptorSetLayoutBindings) const
{
	return vk::DescriptorSetLayoutCreateInfo{
		.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindings.size()),
		.pBindings = descriptorSetLayoutBindings.data()
	};
}

const vk::DescriptorSetLayout& DescriptorSetLayout::getVulkanDescriptorSetLayout() const
{
	return vulkanDescriptorSetLayout;
}