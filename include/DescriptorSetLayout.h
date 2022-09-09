#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class DescriptorSetLayout
{
public:
	explicit DescriptorSetLayout(const vk::Device& vulkanLogicalDevice);
	~DescriptorSetLayout();

	const vk::DescriptorSetLayout& getVulkanDescriptorSetLayout() const;
private:
	vk::DescriptorSetLayoutBinding createUniformBufferDescriptorSetLayoutBinding() const;
	vk::DescriptorSetLayoutBinding createSamplerDescriptorSetLayoutBinding() const;
	vk::DescriptorSetLayoutCreateInfo buildDescriptorSetLayoutCreateInfo(const std::vector<vk::DescriptorSetLayoutBinding>& descriptorSetLayoutBinding) const;

	vk::DescriptorSetLayout vulkanDescriptorSetLayout;
	const vk::Device vulkanLogicalDevice;
};