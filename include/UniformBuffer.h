#pragma once

#include "Buffer.h"
#include "structs/ModelViewProjectionTransformation.h"

class UniformBuffer
{
public:
	UniformBuffer(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize);
	void copyFromCPUToDeviceMemory(const ModelViewProjectionTransformation* data);
	const vk::Buffer getVulkanBuffer() const;

private:
	void createUniformData(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize);
	Buffer buffer;
};