#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class DescriptorPool
{
public:
	DescriptorPool(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight);
	~DescriptorPool();

	const vk::DescriptorPool& getVulkanDescriptorPool() const;

private:
	vk::DescriptorPoolSize buildDescriptorPoolSize(const vk::DescriptorType& descriptorType, const int maxFramesInFlight) const;
	vk::DescriptorPoolCreateInfo buildDescriptorPoolCreateInfo(const std::vector<vk::DescriptorPoolSize>& descriptorPoolSizes, const int maxFramesInFlight) const;

	const vk::Device vulkanLogicalDevice;
	vk::DescriptorPool vulkanDescriptorPool;
};