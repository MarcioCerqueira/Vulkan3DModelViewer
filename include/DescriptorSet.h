#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include "structs/DescriptorSetCreateInfo.h"
#include "ExceptionChecker.h"

class DescriptorSet
{
public:
	explicit DescriptorSet(const DescriptorSetCreateInfo& descriptorSetCreateInfo);
	void write(const vk::Buffer& vulkanUniformBuffer, int frameIndex);

	const vk::DescriptorSet getVulkanDescriptorSet(int frameIndex) const;

private:
	const vk::DescriptorSetAllocateInfo createDescriptorSetAllocateInfo(const DescriptorSetCreateInfo& descriptorSetCreateInfo, std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts) const;
	const vk::DescriptorBufferInfo createBufferInfo(const vk::Buffer& vulkanUniformBuffer) const;
	const vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorBufferInfo& descriptorBufferInfo, int index) const;

	std::vector<vk::DescriptorSet> vulkanDescriptorSets;
	const vk::Device vulkanLogicalDevice;
};