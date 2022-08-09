#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include "CommandBuffer.h"
#include "MemoryProperties.h"
#include "GraphicsQueue.h"

class Buffer
{
public:
	explicit Buffer(const vk::Device& vulkanLogicalDevice);
	~Buffer();
	void createVulkanBuffer(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage);
	void createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags);
	void bindBufferMemory();
	void copyFromCPUToDeviceMemory(const void* data);

	const vk::BufferCreateInfo getBufferCreateInfo() const;
	const vk::Buffer getVulkanBuffer() const;
	
private:
	const vk::BufferCreateInfo buildBufferCreateInfo(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage) const;
	const vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;

	vk::BufferCreateInfo vulkanBufferCreateInfo;
	const vk::Device vulkanLogicalDevice;
	vk::Buffer vulkanBuffer;
	vk::DeviceMemory vulkanBufferMemory;

};
