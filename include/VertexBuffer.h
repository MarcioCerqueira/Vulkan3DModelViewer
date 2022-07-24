#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "CommandBuffer.h"
#include "MemoryProperties.h"
#include "Vertex.h"
#include "VertexBufferCreateInfo.h"

class VertexBuffer
{
public:
	VertexBuffer(const VertexBufferCreateInfo& vertexBufferCreateInfo);
	~VertexBuffer();

	const vk::Buffer getVulkanVertexBuffer() const;
	const size_t getVertexCount() const;

private:
	void createVertexData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	void createStagingData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	vk::Buffer createVulkanBuffer(const std::vector<Vertex>& vertices, const vk::BufferUsageFlags& bufferUsage);
	const vk::BufferCreateInfo buildBufferCreateInfo(const std::vector<Vertex>& vertices, const vk::BufferUsageFlags& bufferUsage) const;
	vk::DeviceMemory createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags);
	const vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;
	void bindBufferMemory(vk::Buffer& buffer, vk::DeviceMemory& memory);
	void copyFromCPUToStagingMemory(const std::vector<Vertex>& vertices);
	void copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue);

	const vk::Device vulkanLogicalDevice;
	vk::Buffer vulkanVertexBuffer;
	vk::Buffer vulkanStagingBuffer;
	vk::DeviceMemory vulkanVertexBufferMemory;
	vk::DeviceMemory vulkanStagingBufferMemory;
	vk::BufferCreateInfo vulkanBufferCreateInfo;
	size_t vertexCount;
};
