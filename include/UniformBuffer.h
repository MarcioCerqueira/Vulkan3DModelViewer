#pragma once

#include "Buffer.h"
#include "structs/ModelViewProjectionTransformation.h"

class UniformBuffer
{
public:
	UniformBuffer(const vk::Device& vulkanLogicalDevice, const PhysicalDeviceProperties& physicalDeviceProperties, const vk::DeviceSize& contentSize);
	void copyFromCPUToDeviceMemory(const ModelViewProjectionTransformation* data);
	const vk::Buffer getVulkanBuffer() const;
	const size_t getSize() const;

private:
	void createUniformData(const PhysicalDeviceProperties& physicalDeviceProperties, const vk::DeviceSize& contentSize);
	Buffer buffer;
	size_t size;
};