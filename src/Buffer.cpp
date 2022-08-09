#include "Buffer.h"

Buffer::Buffer(const vk::Device& vulkanLogicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
}

Buffer::~Buffer()
{
	vulkanLogicalDevice.destroyBuffer(vulkanBuffer);
	vulkanLogicalDevice.freeMemory(vulkanBufferMemory);
}

void Buffer::createVulkanBuffer(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage)
{
	vulkanBufferCreateInfo = buildBufferCreateInfo(contentSize, bufferUsage);
	vulkanBuffer = vulkanLogicalDevice.createBuffer(vulkanBufferCreateInfo);
}

const vk::BufferCreateInfo Buffer::buildBufferCreateInfo(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage) const
{
	return vk::BufferCreateInfo{
		.size = contentSize,
		.usage = bufferUsage,
		.sharingMode = vk::SharingMode::eExclusive
	};
}

void Buffer::createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags)
{
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getBufferMemoryRequirements(vulkanBuffer) };
	const uint32_t memoryTypeIndex{ MemoryProperties::findMemoryType(vulkanPhysicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags) };
	const vk::MemoryAllocateInfo memoryAllocateInfo{ buildMemoryAllocateInfo(memoryRequirements, memoryTypeIndex) };
	vulkanBufferMemory = vulkanLogicalDevice.allocateMemory(memoryAllocateInfo);
}

const vk::MemoryAllocateInfo Buffer::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void Buffer::bindBufferMemory()
{
	const vk::DeviceSize memoryOffset{ 0 };
	vulkanLogicalDevice.bindBufferMemory(vulkanBuffer, vulkanBufferMemory, memoryOffset);
}

void Buffer::copyFromCPUToDeviceMemory(const void* data)
{
	const vk::DeviceSize memoryOffset{ 0 };
	void* mappedData{ vulkanLogicalDevice.mapMemory(vulkanBufferMemory, memoryOffset, vulkanBufferCreateInfo.size) };
	std::memcpy(mappedData, data, static_cast<size_t>(vulkanBufferCreateInfo.size));
	vulkanLogicalDevice.unmapMemory(vulkanBufferMemory);
}

const vk::BufferCreateInfo Buffer::getBufferCreateInfo() const
{
	return vulkanBufferCreateInfo;
}

const vk::Buffer Buffer::getVulkanBuffer() const
{
	return vulkanBuffer;
}