#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "MemoryProperties.h"
#include "Vertex.h"

class VertexBuffer
{
public:
	VertexBuffer(const vk::Device& vulkanLogicalDevice, const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	~VertexBuffer();

	const vk::Buffer getVulkanVertexBuffer() const;
	const size_t getVertexCount() const;

private:
	void createVulkanVertexBuffer(const std::vector<Vertex>& vertices);
	const vk::BufferCreateInfo buildBufferCreateInfo(const std::vector<Vertex>& vertices) const;
	void createVulkanVertexBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice);
	const vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;
	void bindBufferMemory();
	void mapBufferMemory(const std::vector<Vertex>& vertices);

	const vk::Device vulkanLogicalDevice;
	vk::Buffer vulkanVertexBuffer;
	vk::DeviceMemory vulkanVertexBufferMemory;
	vk::BufferCreateInfo vulkanVertexBufferCreateInfo;
	int vertexCount;
};
