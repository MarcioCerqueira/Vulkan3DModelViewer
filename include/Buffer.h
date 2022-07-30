#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include "CommandBuffer.h"
#include "MemoryProperties.h"
#include "GraphicsQueue.h"

class Buffer
{
public:
	const vk::Buffer getVulkanBuffer() const;

protected:
	Buffer(const vk::Device& vulkanLogicalDevice);
	virtual ~Buffer();

	template<typename T>
	void createStagingData(const std::vector<T>& content, const vk::PhysicalDevice& vulkanPhysicalDevice)
	{
		const vk::BufferUsageFlags stagingBufferUsage{ vk::BufferUsageFlagBits::eTransferSrc };
		vulkanStagingBuffer = createVulkanBuffer<T>(content, stagingBufferUsage);
		const vk::MemoryPropertyFlags stagingMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
		vulkanStagingBufferMemory = createVulkanBufferMemory(vulkanPhysicalDevice, stagingMemoryPropertyFlags);
		bindBufferMemory(vulkanStagingBuffer, vulkanStagingBufferMemory);
	}

	template <typename T>
	vk::Buffer createVulkanBuffer(const std::vector<T>& content, const vk::BufferUsageFlags& bufferUsage)
	{
		vulkanBufferCreateInfo = buildBufferCreateInfo<T>(content, bufferUsage);
		return vulkanLogicalDevice.createBuffer(vulkanBufferCreateInfo);
	}

	vk::DeviceMemory createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags);

	void bindBufferMemory(vk::Buffer& buffer, vk::DeviceMemory& memory);
	
	template <typename T>
	void copyFromCPUToStagingMemory(const std::vector<T>& content)
	{
		const vk::DeviceSize memoryOffset{ 0 };
		void* data{ vulkanLogicalDevice.mapMemory(vulkanStagingBufferMemory, memoryOffset, vulkanBufferCreateInfo.size) };
		std::memcpy(data, content.data(), static_cast<size_t>(vulkanBufferCreateInfo.size));
		vulkanLogicalDevice.unmapMemory(vulkanStagingBufferMemory);
	}
	
	void copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue);

	vk::Buffer vulkanBuffer;
	vk::DeviceMemory vulkanBufferMemory;
	
private:

	const vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;

	template <typename T>
	const vk::BufferCreateInfo buildBufferCreateInfo(const std::vector<T>& content, const vk::BufferUsageFlags& bufferUsage) const
	{
		return vk::BufferCreateInfo{
			.size = sizeof(content[0]) * content.size(),
			.usage = bufferUsage,
			.sharingMode = vk::SharingMode::eExclusive
		};
	}

	vk::Buffer vulkanStagingBuffer;
	vk::DeviceMemory vulkanStagingBufferMemory;
	vk::BufferCreateInfo vulkanBufferCreateInfo;
	const vk::Device vulkanLogicalDevice;

};
