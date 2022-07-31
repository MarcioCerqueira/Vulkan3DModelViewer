#pragma once

#include "Buffer.h"

class UniformBuffer : public Buffer
{
public:
	UniformBuffer(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize);
private:
	void createUniformData(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::DeviceSize& contentSize);
};