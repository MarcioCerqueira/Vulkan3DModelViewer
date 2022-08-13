#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class Sampler
{
public:
	Sampler(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDeviceProperties& physicalDeviceProperties);
	~Sampler();

private:
	const vk::SamplerCreateInfo buildSamplerCreateInfo(const vk::PhysicalDeviceProperties& physicalDeviceProperties) const;

	const vk::Device vulkanLogicalDevice;
	vk::Sampler vulkanSampler;
};