#pragma once

#include "Buffer.h"
#include "Image.h"

class StagingBuffer
{
public:
	explicit StagingBuffer(const vk::Device& vulkanLogicalDevice);
	void createStagingData(const vk::DeviceSize& contentSize, const vk::PhysicalDevice& vulkanPhysicalDevice);
	void copyFromCPUToStagingMemory(const void* data);
	void copyFromStagingToDeviceMemory(const std::shared_ptr<CommandBuffer>& commandBuffers, const vk::Buffer& dstBuffer);
	void copyFromStagingToDeviceMemory(const std::shared_ptr<CommandBuffer>& commandBuffers, const std::shared_ptr<Image>& dstImage);

private:

	Buffer buffer;
	const vk::Device vulkanLogicalDevice;
};