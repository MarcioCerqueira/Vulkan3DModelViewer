#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const vk::Device& vulkanLogicalDevice, const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	createVulkanVertexBuffer(vertices);
	createVulkanVertexBufferMemory(vulkanPhysicalDevice);
	bindBufferMemory();
	mapBufferMemory(vertices);
}

VertexBuffer::~VertexBuffer()
{
	vulkanLogicalDevice.destroyBuffer(vulkanVertexBuffer);
	vulkanLogicalDevice.freeMemory(vulkanVertexBufferMemory);
}

void VertexBuffer::createVulkanVertexBuffer(const std::vector<Vertex>& vertices)
{
	vertexCount = vertices.size();
	vulkanVertexBufferCreateInfo = buildBufferCreateInfo(vertices);
	vulkanVertexBuffer = vulkanLogicalDevice.createBuffer(vulkanVertexBufferCreateInfo);
}

const vk::BufferCreateInfo VertexBuffer::buildBufferCreateInfo(const std::vector<Vertex>& vertices) const
{
	return vk::BufferCreateInfo{
		.size = sizeof(vertices[0]) * vertices.size(),
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.sharingMode = vk::SharingMode::eExclusive
	};
}

void VertexBuffer::createVulkanVertexBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getBufferMemoryRequirements(vulkanVertexBuffer) };
	const vk::MemoryPropertyFlags memoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	const uint32_t memoryTypeIndex{ MemoryProperties::findMemoryType(vulkanPhysicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags) };
	const vk::MemoryAllocateInfo memoryAllocateInfo{ buildMemoryAllocateInfo(memoryRequirements, memoryTypeIndex) };
	vulkanVertexBufferMemory = vulkanLogicalDevice.allocateMemory(memoryAllocateInfo);
}

const vk::MemoryAllocateInfo VertexBuffer::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void VertexBuffer::bindBufferMemory()
{
	const vk::DeviceSize memoryOffset{ 0 };
	vulkanLogicalDevice.bindBufferMemory(vulkanVertexBuffer, vulkanVertexBufferMemory, memoryOffset);
}

void VertexBuffer::mapBufferMemory(const std::vector<Vertex>& vertices)
{
	const vk::DeviceSize memoryOffset{ 0 };
	void* data{ vulkanLogicalDevice.mapMemory(vulkanVertexBufferMemory, memoryOffset, vulkanVertexBufferCreateInfo.size) };
	std::memcpy(data, vertices.data(), static_cast<size_t>(vulkanVertexBufferCreateInfo.size));
	vulkanLogicalDevice.unmapMemory(vulkanVertexBufferMemory);
}

const vk::Buffer VertexBuffer::getVulkanVertexBuffer() const
{
	return vulkanVertexBuffer;
}

const size_t VertexBuffer::getVertexCount() const
{
	return vertexCount;
}