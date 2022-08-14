#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "ExceptionChecker.h"
#include "Image.h"
#include "UniformBuffer.h"

class DescriptorSet
{
public:
	DescriptorSet(const vk::Device& vulkanLogicalDevice, const int maxFramesInFlight);
	void write(const std::shared_ptr<UniformBuffer>& uniformBuffer, const std::shared_ptr<Image>& image, int frameIndex);
	
	const vk::DescriptorSet getVulkanDescriptorSet(int frameIndex) const;
	const vk::DescriptorSetLayout getVulkanDescriptorSetLayout() const;

private:
	const vk::DescriptorSetAllocateInfo createDescriptorSetAllocateInfo(const vk::DescriptorPool& vulkanDescriptorPool, const int maxFramesInFlight, std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts) const;
	const vk::DescriptorBufferInfo createBufferInfo(const vk::Buffer& vulkanUniformBuffer, const size_t uniformBufferObjectSize) const;
	const vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorBufferInfo& descriptorBufferInfo, int index) const;
	const vk::DescriptorImageInfo createImageInfo(const std::shared_ptr<Image>& image) const;
	const vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorImageInfo& descriptorImageInfo, int index) const;

	std::vector<vk::DescriptorSet> vulkanDescriptorSets;
	const vk::Device vulkanLogicalDevice;
	const DescriptorPool descriptorPool;
	const DescriptorSetLayout layout;
};