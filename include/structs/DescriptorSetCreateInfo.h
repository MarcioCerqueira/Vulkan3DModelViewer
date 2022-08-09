#pragma once

#include <vulkan/vulkan.hpp>
struct DescriptorSetCreateInfo
{
	vk::Device vulkanLogicalDevice;
	vk::DescriptorPool vulkanDescriptorPool;
	vk::DescriptorSetLayout vulkanDescriptorSetLayout;
	int maxFramesInFlight;
};