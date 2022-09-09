#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class Sampler
{
public:
	Sampler(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDeviceProperties& physicalDeviceProperties, const uint32_t mipLevels);
	~Sampler();

	const vk::Sampler& getVulkanSampler() const;

private:
	vk::SamplerCreateInfo buildSamplerCreateInfo(const vk::PhysicalDeviceProperties& physicalDeviceProperties, const uint32_t mipLevels) const;

	const vk::Device vulkanLogicalDevice;
	vk::Sampler vulkanSampler;
};