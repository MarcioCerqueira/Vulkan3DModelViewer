#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class DescriptorPool
{
public:
	DescriptorPool(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight);
	~DescriptorPool();

	const vk::DescriptorPool getVulkanDescriptorPool() const;

private:
	const vk::DescriptorPoolSize buildDescriptorPoolSize(const int maxFramesInFlight) const;
	const vk::DescriptorPoolCreateInfo buildDescriptorPoolCreateInfo(const vk::DescriptorPoolSize& descriptorPoolSize, const int maxFramesInFlight) const;

	const vk::Device vulkanLogicalDevice;
	vk::DescriptorPool vulkanDescriptorPool;
};