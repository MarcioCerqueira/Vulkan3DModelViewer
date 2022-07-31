#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class DescriptorSetLayout
{
public:
	explicit DescriptorSetLayout(const vk::Device& vulkanLogicalDevice);
	~DescriptorSetLayout();

	vk::DescriptorSetLayout getVulkanDescriptorSetLayout() const;
private:
	const vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;
	const vk::DescriptorSetLayoutCreateInfo buildDescriptorSetLayoutCreateInfo(const vk::DescriptorSetLayoutBinding& descriptorSetLayoutBinding) const;

	vk::DescriptorSetLayout vulkanDescriptorSetLayout;
	const vk::Device vulkanLogicalDevice;
};