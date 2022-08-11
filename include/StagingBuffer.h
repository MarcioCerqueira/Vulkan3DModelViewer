#pragma once

#include "Buffer.h"
#include "Image.h"

class StagingBuffer
{
public:
	explicit StagingBuffer(const vk::Device& vulkanLogicalDevice);
	void createStagingData(const vk::DeviceSize& contentSize, const vk::PhysicalDevice& vulkanPhysicalDevice);
	void copyFromCPUToStagingMemory(const void* data);
	void copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue, const vk::Buffer& dstBuffer);
	void copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue, const std::shared_ptr<Image>& dstImage);

private:

	Buffer buffer;
	const vk::Device vulkanLogicalDevice;
};